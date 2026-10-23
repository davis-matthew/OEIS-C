/*
 * A104098  a(n) = Sum_{k=1..n} binomial(n-1, k-1)*A008292(n, k) for n >= 1.
 * https://oeis.org/A104098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1) * A008292(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Σ_{j=0}^{n-1} C(n-1,j) * Eulerian(n,j)       */

#include <stdlib.h>
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Allocate two rows of Eulerian numbers: prev (size n) and curr (size n). */
    seq_t *prev = seq_malloc(n);
    seq_t *curr = seq_malloc(n);

    /* Base row for n = 1: E(1,0) = 1. */
    prev[0] = 1;

    for (idx_t m = 2; m <= n; ++m) {
        /* Compute the row for n = m: E(m,j) for j = 0..m-1. */
        for (idx_t j = 0; j < m; ++j) {
            seq_t t1 = 0, t2 = 0;
            if (j < m - 1)  /* (j+1) * E(m-1,j) */
                t1 = (seq_t)(j + 1) * prev[j];
            if (j > 0)      /* (m-j) * E(m-1,j-1) */
                t2 = (seq_t)(m - j) * prev[j - 1];
            curr[j] = t1 + t2;
        }

        if (m == n) {
            /* Convolution with binomial coefficients.  bin = C(m-1,0)=1. */
            seq_t sum = 0;
            seq_t bin = 1;
            for (idx_t j = 0; j < m; ++j) {
                sum += bin * curr[j];
                if (j < m - 1)
                    bin = bin * (seq_t)(m - 1 - j) / (seq_t)(j + 1);
            }
            free(prev);
            free(curr);
            return sum;
        }

        /* Prepare for next iteration.  Swap the two rows. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Should never reach here. */
    free(prev);
    free(curr);
    return 0;
}
