/*
 * A066447  Number of basis partitions (or basic partitions) of n.
 * https://oeis.org/A066447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{⌊√n⌋} coeff of x^{n−k²} in * ∏_{i=1}^{k}
 *            (1+x^i)/(1−x^i)
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 199, u64 n <= 668,
 *            u128 n <= 2389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N = n;

    /* Allocate working arrays, initially zeroed. */
    seq_t *f   = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *tmp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *a   = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));

    if (!f || !tmp || !a) return 0;            /* Allocation failure */

    /* Q_0(x) = 1 */
    f[0] = 1;

    /* Max k needed: floor(sqrt(N)) */
    idx_t maxK = 0;
    while ((maxK + 1) * (maxK + 1) <= N) ++maxK;

    for (idx_t k = 0; k <= maxK; ++k)
    {
        /* Add contribution of Q_k to a(n) */
        idx_t k2 = k * k;
        for (idx_t s = 0; (k2 + s) <= N; ++s)
            a[k2 + s] += f[s];

        if (k == maxK) break;   /* finished all needed k */

        /* Compute Q_{k+1}(x) from Q_k(x) using
         * (1 + x^{k+1})/(1 - x^{k+1})
         * The new coefficient array is stored in tmp.
         */
        idx_t w = k + 1;               /* part size */
        for (idx_t r = 0; r < w; ++r)
        {
            seq_t prefix = 0;
            for (idx_t idx = r; idx <= N; idx += w)
            {
                /* tmp[idx] = f[idx] + 2 * prefix where
                 * prefix = Σ_{j<idx, j≡r mod w} f[j]
                 */
                tmp[idx] = f[idx] + (seq_t)2 * prefix;
                prefix += f[idx];
            }
        }

        /* Swap the old and new coefficient arrays. */
        seq_t *swap = f; f = tmp; tmp = swap;
    }

    seq_t result = a[N];

    /* Free temporary storage. */
    free(f);
    free(tmp);
    free(a);

    return result;
}
