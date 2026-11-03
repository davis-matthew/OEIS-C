/*
 * A142137  Primes congruent to 28 mod 37.
 * https://oeis.org/A142137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the n-th prime p such that p ≡ 28 (mod 37).
   We generate primes by testing successive integers with is_prime_big,
   storing those that satisfy the congruence.  Results are cached
   so that arbitrary order of calls is handled efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cached     = NULL;
    static size_t cached_cap = 0;
    static size_t cached_sz  = 0;

    /* smallest integer to start checking from */
    static seq_t candidate = 2;

    /* offset is 1, values below are irrelevant */
    if (n == 0) return 0;

    /* return cached value if already computed */
    if ((size_t)n <= cached_sz)
        return cached[n - 1];

    /* initialise cache on first use */
    if (cached_cap == 0) {
        cached_cap = 8;
        cached = mem_new(seq_t, cached_cap);
        if (!cached) abort();          /* allocation failure */
    }

    /* generate primes until we have the n-th one */
    while (cached_sz < (size_t)n) {
        /* find the next prime */
        while (!is_prime_big(candidate)) {
            ++candidate;
        }

        /* if it satisfies the congruence, store it */
        if (candidate % (seq_t)37 == (seq_t)28) {
            if (cached_sz >= cached_cap) {       /* grow array */
                cached_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cached, cached_cap);
                if (!tmp) abort();
                cached = tmp;
            }
            cached[cached_sz++] = candidate;
        }

        /* move on to the next integer after the current prime */
        ++candidate;
    }

    return cached[n - 1];
}
