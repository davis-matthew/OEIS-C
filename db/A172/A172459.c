/*
 * A172459  Primes p such that exactly one of the numbers 2p-1 and 2^p - 1 is
 *          prime.
 * https://oeis.org/A172459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime p such that exactly one of 2p‑1 and 2^p‑1 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* List of exponents e for which 2^e‑1 is Mersenne prime.
   This list contains all known exponents up to the current record (82589933).
   Any prime p not in this list will give a composite 2^p‑1.  */
static const idx_t mersenne_exponents[] = {
    2ULL,   3ULL,   5ULL,   7ULL,  13ULL,  17ULL,  19ULL,  31ULL,
   61ULL,  89ULL, 107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL,
  2281ULL, 3217ULL, 4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL,
 21701ULL, 23209ULL, 44497ULL, 86243ULL, 110503ULL, 132049ULL, 216091ULL,
 756839ULL, 859433ULL, 1257787ULL, 1398269ULL, 2976221ULL, 3021377ULL,
 3258267ULL, 3715667ULL, 4264387ULL, 4311263ULL, 5788517ULL, 7420721ULL,
 7723297ULL, 82589933ULL
};
static const size_t mersenne_cnt = sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

/* Binary search in the above table. */
static bool is_mersenne_exponent(idx_t p)
{
    size_t lo = 0, hi = mersenne_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (mersenne_exponents[mid] == p) return true;
        if (mersenne_exponents[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Cached sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next integer to test for primality. */
static idx_t next_candidate = 2;

/* Return the n-th term (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined but harmless */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate)) ++next_candidate;
        idx_t p = next_candidate;
        ++next_candidate;             /* prepare for the next call */

        /* Test the two conditions. */
        bool two_p_minus_one_prime = is_prime(p * 2ULL - 1ULL);
        bool two_pow_p_minus_one_prime = is_mersenne_exponent(p);

        /* Exactly one of them must be prime. */
        if (two_p_minus_one_prime ^ two_pow_p_minus_one_prime) {
            /* Ensure enough space. */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the term. */
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[(size_t)n - 1];
}
