/*
 * A161691  Number of reduced words of length n in the Weyl group A_46.
 * https://oeis.org/A161691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{46} (1 - x^(k+1)) / (1 -
 *            x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine implements the generating function by successive
   multiplication by (1 - x^(k+1)) and division by (1 - x) using only
   non‑negative integer arithmetic.  The result fits in seq_t for all
   0 ≤ n ≤ 10000. */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"  /* provided helper */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return (seq_t)1;

    /* Allocate working arrays of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *f = alloc_seq_array(len);
    seq_t *g = alloc_seq_array(len);
    seq_t *cum = alloc_seq_array(len);

    /* initialize f: f[0] = 1, others = 0 */
    for (size_t i = 0; i < len; ++i) f[i] = 0;
    f[0] = (seq_t)1;

    for (idx_t k = 1; k <= 46; ++k) {
        /* compute cumulative sums of f  : cum[i] = Σ_{j=0}^{i} f[j] */
        seq_t acc = 0;
        for (size_t i = 0; i < len; ++i) {
            acc += f[i];
            cum[i] = acc;
        }

        /* produce g = (1 - x^{k+1}) * f followed by division by (1 - x) */
        for (size_t i = 0; i < len; ++i) {
            seq_t val = cum[i];
            if (i >= (size_t)(k + 1))
                val -= cum[i - (size_t)(k + 1)];
            g[i] = val;
        }

        /* swap f and g for next iteration */
        seq_t *tmp = f;
        f = g;
        g = tmp;
    }

    /* The answer is the nth coefficient */
    seq_t result = f[n];

    /* free temporary arrays */
    free(cum);
    free(g);   /* g contains the old f */
    free(f);   /* f is no longer needed */

    return result;
}
