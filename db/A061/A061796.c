/*
 * A061796  Number of distinct sums sigma(i) + sigma(j) for 1<=i<=j<=n, where
 *          sigma(k) = A000203(k).
 * https://oeis.org/A061796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of distinct sums sigma(i)+sigma(j)
   for 1 ≤ i ≤ j ≤ n, where sigma(k) is the sum of divisors of k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* 1. Compute sigma(1)..sigma(n) and find maximum */
    seq_t *sigma = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!sigma)
        return 0;

    seq_t max_sigma = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_of(i);
        sigma[i - 1] = s;
        if (s > max_sigma)
            max_sigma = s;
    }

    /* 2. Array to record which sums have appeared.
       Maximum possible sum is 2*max_sigma. */
    size_t sum_limit = (size_t)max_sigma * 2 + 1;
    unsigned char *present = (unsigned char *)calloc(sum_limit, 1);
    if (!present) {
        free(sigma);
        return 0;
    }

    /* 3. Enumerate all i ≤ j and count distinct sums */
    seq_t distinct = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t si = sigma[i];
        for (idx_t j = i; j < n; ++j) {
            seq_t sum = si + sigma[j];
            size_t idx = (size_t)sum;
            if (!present[idx]) {
                present[idx] = 1;
                ++distinct;
            }
        }
    }

    free(present);
    free(sigma);
    return distinct;
}
