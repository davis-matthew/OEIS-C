/*
 * A234925  Primes which become squares when the digits are rotated once to
 *          the right.
 * https://oeis.org/A234925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Search for primes whose right rotation is a perfect square.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* static storage for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t generated = 0;     /* number of terms already generated */
static seq_t last_candidate = 2;/* next integer to test for primality */

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0, return 0 */

    while (generated < n) {
        if (is_prime_big(last_candidate)) {
            seq_t d = last_candidate % 10;          /* last decimal digit */
            int  len = num_digits(last_candidate);   /* number of digits     */

            /* term after right rotation: d * 10^(len-1) + floor(p/10) */
            seq_t pow10len = pow10((idx_t)(len - 1));
            seq_t rotated = d * pow10len + (last_candidate / 10);

            seq_t sq = floor_sqrt(rotated);
            if (sq * sq == rotated) {               /* rotated is a square */
                if (generated >= cache_cap) {        /* grow cache if needed */
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
                    if (!new_ptr) abort();           /* allocation failed */
                    cache = new_ptr;
                    cache_cap = new_cap;
                }
                cache[generated] = last_candidate;   /* store term */
                ++generated;
            }
        }
        last_candidate = last_candidate + 1; /* check next integer */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}
