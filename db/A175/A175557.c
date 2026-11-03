/*
 * A175557  Prime preperiodic part of the decimal expansion of 1/k as k runs
 *          through A065502.
 * https://oeis.org/A175557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 66, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A175557.
   A(n) is the nth prime that occurs as the integer preperiodic part
   of the decimal expansion of 1/k for k that are multiples of 2 or 5.
   For each such k let
        L = max(v2(k), v5(k))          // maximum exponent of 2 or 5 in k
   then the preperiodic part is
        p = floor(10^L / k)
   If p is prime it is recorded as the next element of the sequence. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of the primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next k to test (smallest multiple of 2 or 5 not yet examined) */
    static idx_t next_k = 2;

    if (n == 0) return 0;          /* undefined for n <= 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* skip k that are not multiples of 2 or 5 */
        while ((next_k % 2ULL != 0) && (next_k % 5ULL != 0))
            ++next_k;

        idx_t k = next_k;

        /* count powers of 2 and 5 in k */
        idx_t tmp = k;
        idx_t exp2 = 0;
        idx_t exp5 = 0;
        while ((tmp & 1ULL) == 0) { ++exp2; tmp >>= 1; }
        while (tmp % 5ULL == 0)  { ++exp5; tmp /= 5ULL; }
        idx_t m = tmp;  /* the remaining factor (gcd(m,10)==1) */

        /* L is the maximum of the 2‑ and 5‑exponents */
        idx_t L = (exp2 > exp5 ? exp2 : exp5);

        /* compute the preperiodic part p = floor(10^L / k) */
        seq_t num = pow10((idx_t)L);     /* 10^L as seq_t */
        seq_t p   = num / (seq_t)k;      /* integer division */

        /* record p if it is prime */
        if (is_prime_big(p))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) /* out of memory: abort with 0 */
                    return 0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }

        ++next_k;   /* proceed to next candidate */
    }

    return cache[n - 1];
}
