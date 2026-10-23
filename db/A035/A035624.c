/*
 * A035624  Number of partitions of n into parts 4k+1 and 4k+2 with at least
 *          one part of each type.
 * https://oeis.org/A035624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 4k+1 and 4k+2 * with
 *            at least one part of each type
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 224, u64 n <= 763,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a( n ) as defined by OEIS sequence A035624. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Temporary DP arrays.  size_t is safe for indices up to n (≤ 10000). */
    size_t N = (size_t)n + 1;
    seq_t *f  = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *g1 = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *g2 = (seq_t *)calloc(N, sizeof(seq_t));
    if (!f || !g1 || !g2) {
        /* memory exhausted – return 0 (sequence not needed in this case). */
        free(f); free(g1); free(g2);
        return 0;
    }

    /* Base value for the empty partition. */
    f[0]  = 1;
    g1[0] = 1;
    g2[0] = 1;

    /* Standard coin‑change DP: unlimited copies of each part. */
    for (idx_t p = 1; p <= n; ++p) {
        if ((p & 3) == 1 || (p & 3) == 2) {
            for (idx_t i = p; i <= n; ++i) {
                f[i]  = f[i] + f[i - p];
                if ((p & 3) == 1)
                    g1[i] = g1[i] + g1[i - p];
                else
                    g2[i] = g2[i] + g2[i - p];
            }
        }
    }

    seq_t ans = f[n];
    ans = ans - g1[n];
    ans = ans - g2[n];

    free(f); free(g1); free(g2);
    return ans;
}
