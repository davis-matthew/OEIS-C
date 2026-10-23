/*
 * A118302  Number of partitions of n into distinct parts with largest part
 *          not congruent to n modulo 2.
 * https://oeis.org/A118302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with *
 *            largest part not congruent to n modulo 2. This is computed by *
 *            dynamic programming tracking the parity of the largest part
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The build supplies integer types seq_t (value) and idx_t (index). */

static idx_t max_n = 0;       /* maximum n that has been precomputed */
static seq_t *even_parts = NULL;  /* counts where largest part is even */
static seq_t *odd_parts  = NULL;  /* counts where largest part is odd */

seq_t a(idx_t n)
{
    /* n is always >= 1 in the problem, but guard anyway */
    if (n == 0) return 0;

    /* Compute the table up to the requested n if needed. */
    if (n > max_n) {
        /* Allocate new tables of size n+1. */
        seq_t *new_even = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        seq_t *new_odd  = (seq_t*)malloc((n + 1) * sizeof(seq_t));

        /* Initialize to zero. */
        for (idx_t i = 0; i <= n; ++i) {
            new_even[i] = 0;
            new_odd[i]  = 0;
        }

        /* Dynamic programming over distinct parts, tracking parity
         * of the largest part.  We process parts in increasing order,
         * so when part p is added, all previous parts are smaller than p. */
        for (idx_t p = 1; p <= n; ++p) {
            idx_t parity = p & 1;   /* 0 = even, 1 = odd */
            for (idx_t i = n; i >= p; --i) {
                seq_t add = new_even[i - p] + new_odd[i - p];
                if (i == p)   /* partition consisting solely of part p */
                    add = 1;

                if (parity == 0)
                    new_even[i] += add;
                else
                    new_odd[i]  += add;
            }
        }

        /* Free old tables, keep the new ones. */
        if (even_parts) free(even_parts);
        if (odd_parts)  free(odd_parts);
        even_parts = new_even;
        odd_parts  = new_odd;
        max_n = n;
    }

    /* Return the count with the required parity of the largest part. */
    return (n & 1) ? even_parts[n] : odd_parts[n];
}
