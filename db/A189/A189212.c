/*
 * A189212  Zero-one sequence based on the hexagonal numbers:
 *          a(A000384(k))=a(k); a(v(k))=1-a(k), where v=A183218 and a(1)=0.
 * https://oeis.org/A189212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(A000384(k)) = a(k); a(v(k)) = 1 - a(k); a(1) = 0. Here
 *            A000384(k) = 2k²-k (hexagonal numbers) and v(k) is the smallest
 *            integer > k that has not been assigned yet. This implements the
 *            explicit construction of OEIS A189212
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate arrays up to n (inclusive) */
    seq_t *seq   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    unsigned char *vis = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!seq || !vis) {
        free(seq);
        free(vis);
        return (seq_t)0;               /* failure fallback */
    }

    for (idx_t i = 0; i <= n; ++i) vis[i] = 0;

    seq[1] = (seq_t)0;                  /* a(1) = 0 */
    vis[1] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak = seq[k];                /* current value of a(k) */

        /* a(A000384(k)) = a(k) for hexagonal numbers */
        idx_t h = 2 * k * k - k;          /* 2k^2 - k */
        if (h <= n) {
            seq[h] = ak;
            vis[h] = 1;
        }

        /* find the next unassigned index j > k */
        idx_t j = k + 1;
        while (j <= n && vis[j]) ++j;
        if (j <= n) {
            seq[j] = (ak ? 0 : 1);        /* a(v(k)) = 1 - a(k) */
            vis[j] = 1;
        }
    }

    seq_t result = seq[n];
    free(seq);
    free(vis);
    return result;
}
