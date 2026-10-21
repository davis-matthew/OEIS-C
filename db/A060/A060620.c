/*
 * A060620  Average of the first n primes rounded down.
 * https://oeis.org/A060620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (p1 + p2 + … + pn) / n ) where pk is the k‑th
 *            prime
 * Range:     u8 n <= 13, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the average of the first n primes, rounded down. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* cumulative sums: cum_sum[i] = sum of first i primes, cum_sum[0] = 0 */
    static seq_t *cum_sum = NULL;
    static size_t capacity = 0;    /* allocated size (in seq_t elements) */
    static idx_t computed_n = 0;   /* highest n with cum_sum[0..n] computed */

    /* Do nothing for n == 0; result is undefined but harmless. */
    if (n == 0) return (seq_t)0;

    /* Expand storage if needed and compute missing terms. */
    if (n > computed_n) {
        /* Ensure the array can hold indices 0..n. */
        if (capacity < (size_t)(n + 1)) {
            size_t new_cap = capacity ? capacity * 2 : 64;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            /* cast is required for C++ compilation */
            seq_t *tmp = (seq_t *)realloc(cum_sum, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failure should never happen */
            cum_sum = tmp;
            capacity = new_cap;
        }

        /* Base case: sum of 0 primes is 0. */
        if (computed_n == 0)
            cum_sum[0] = (seq_t)0;

        /* Compute primes and cumulative sums up to n. */
        for (idx_t i = computed_n + 1; i <= n; ++i) {
            seq_t prime = get_prime(i);          /* prime number */
            cum_sum[i] = cum_sum[i - 1] + prime; /* cumulative sum */
        }
        computed_n = n;
    }

    /* Integer division truncates toward 0, which is floor for positive values. */
    return cum_sum[n] / (seq_t)n;
}
