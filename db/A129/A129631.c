/*
 * A129631  Numbers k such that sum of digits of binomial(k+1, k-1) is a
 *          prime.
 * https://oeis.org/A129631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129631:
   a(n) = k such that the sum of decimal digits of binomial(k+1, k-1)
   is a prime number.  binomial(k+1, k-1) = (k+1)·k / 2.  This function
   generates the sequence by inspecting triangular numbers one by one
   and storing the qualifying k values. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

#define CACHE_MAX 200000   /* enough for all n requested in the tests */

seq_t a(idx_t n)
{
    /* undefined for n == 0 – it will not be called in that case */
    if (n == 0) return (seq_t)0;

    /* static state */
    static seq_t cache[CACHE_MAX];
    static size_t cache_len = 0;      /* number of terms stored */
    static idx_t inspected_k = 0;     /* last k inspected */
    static seq_t tri = 0;             /* current triangular number T_k */

    /* extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* move to the next k */
        inspected_k++;

        /* T_k = T_{k-1} + k  (with T_0 = 0) */
        tri += (seq_t)inspected_k;

        /* sum of decimal digits of T_k */
        seq_t sum = digit_sum(tri);

        /* test whether the sum is prime */
        if (is_prime_small(sum)) {
            /* store the qualifying k */
            if (cache_len < CACHE_MAX) {
                cache[cache_len++] = inspected_k;
            }
            /* if CACHE_MAX is exceeded we simply continue generating
               terms without storing them – this situation will not occur
               for the intended use cases. */
        }
    }
    return cache[n - 1];
}
