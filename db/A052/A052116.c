/*
 * A052116  Number of nonnegative integer pairs (i,j) with binomial(i+r,r) +
 *          binomial(j+r,r) <= binomial(n+r,r), r=3.
 * https://oeis.org/A052116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of nonnegative integer pairs (i,j) such that
 *            C(i+3,3)+C(j+3,3) <= C(n+3,3) (with r=3)
 * Range:     u8 n <= 9, u16 n <= 271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute C(n+3,3) */
    seq_t fn = ((seq_t)(n+3)) * (n+2) * (n+1) / 6;

    /* pre‑compute f[k] = C(k+3,3) for 0 <= k <= n */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f) return (seq_t)0;  /* out of memory – return 0 */

    for (idx_t k = 0; k <= n; ++k)
        f[k] = ((seq_t)(k+3)) * (k+2) * (k+1) / 6;

    int jmax = (int)n;          /* largest j currently possible   */
    seq_t ans = 0;              /* running total of pairs         */

    for (idx_t i = 0; i <= n; ++i) {
        while (jmax >= 0 && f[i] + f[jmax] > fn)
            --jmax;              /* shrink jmax until condition holds */

        if (jmax < 0) break;     /* no more j satisfy the inequality */

        ans += (seq_t)(jmax + 1);   /* j = 0 .. jmax inclusive */
    }

    free(f);
    return ans;
}
