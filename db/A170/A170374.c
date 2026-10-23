/*
 * A170374  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = N_n + 27·(a[n-1]+…+a[n-42]) –
 *            378·a[n-43], where N_n = 1 (n=0), 2 (1≤n≤42), 1 (n=43), 0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   N_of, alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/N_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate an array of a[0]…a[n]. */
    seq_t *a = alloc_seq_array((size_t)n + 1);

    a[0] = (seq_t)1;
    seq_t sum_last42 = a[0];          /* sum of a[n-1]…a[n-42] for the current n */

    for (idx_t k = 1; k <= n; ++k) {
        /* 27 * (a[k-1] + … + a[max(0, k-42)]) */
        seq_t sum27 = sum_last42 * (seq_t)27;

        /* a[k] = N_k + sum27 - 378 * a[k-43] (if k ≥ 43) */
        seq_t val = N_of(k) + sum27;
        if (k >= 43) {
            val -= a[k - 43] * (seq_t)378;
        }
        a[k] = val;

        /* Update the sliding window for the next iteration. */
        sum_last42 += a[k];
        if (k >= 42) {
            sum_last42 -= a[k - 42];
        }
    }

    seq_t result = a[n];
    free_seq_array(a);
    return result;
}
