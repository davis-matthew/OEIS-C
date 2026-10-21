/*
 * A201998  Positive numbers n such that n^2 + n + 41 is composite and there
 *          are no positive integers c such that n = c*x^2 + (c + 1)*x + c*41
 *          for an integer x.
 * https://oeis.org/A201998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer n such that * d = n^2 + n + 41
 *            is composite and there is no positive integer c * and integer x
 *            for which n = c*x^2 + (c + 1)*x + c*41
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache      = NULL;   /* allocated array of terms */
    static size_t  cache_cap = 0;      /* current capacity of cache */
    static idx_t   cache_len = 0;      /* number of terms stored */
    static seq_t   next_candidate = 1; /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = next_candidate++;   /* candidate integer to test */

        /* 1. Check that cand^2 + cand + 41 is composite */
        seq_t val = cand * cand + cand + 41;
        if (is_prime_big(val)) {
            continue;            /* val is prime, skip this candidate */
        }

        /* 2. Ensure cand cannot be written as c*x^2 + (c+1)*x + c*41 */
        bool has_rep = false;

        /* 2a.  x >= 0 : n = c*q + x, q = x^2 + x + 41 */
        for (seq_t x = 0; x <= cand; ++x) {
            seq_t q   = x * x + x + 41;
            seq_t lhs = cand - x;          /* cand - x is safe because x <= cand */

            if (q > lhs) break;            /* no further possibilities */
            if ((lhs % q) == 0) {
                has_rep = true;
                break;
            }
        }

        /* 2b.  x < 0 : let y = -x > 0, then n = c*s - y, s = y^2 - y + 41 */
        if (!has_rep) {
            for (seq_t y = 1; ; ++y) {
                seq_t s   = y * y - y + 41;
                seq_t rhs = cand + y;          /* n + y > n */

                if (s > rhs) break;
                if ((rhs % s) == 0) {
                    has_rep = true;
                    break;
                }
            }
        }

        if (!has_rep) {
            /* keep the term in the cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
