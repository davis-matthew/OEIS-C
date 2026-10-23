/*
 * A169176  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17 * Σ_{k=1}^{min(n,26)} a(n-k) – 153 * a(n-27) + p_n *
 *            where p_0 = 1, p_1 … p_26 = 2, p_27 = 1, and p_n = 0 for n > 27.
 *            * This recurrence follows from the rational generating function
 *            of A169176
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *seq = NULL;
    static size_t  seq_len = 0;   /* number of elements currently stored */
    static size_t  seq_cap = 0;   /* allocated capacity */

    /* Allocate or expand cache if needed */
    if (n >= seq_len) {
        size_t needed = n + 1;
        if (needed > seq_cap) {
            seq = (seq_t*)realloc(seq, needed * sizeof(seq_t));
            seq_cap = needed;
        }
        /* Compute new terms up to n */
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t pi = 0;
            if (i == 0)            pi = 1;      /* p_0 */
            else if (i <= 26)      pi = 2;      /* p_1 … p_26 */
            else if (i == 27)      pi = 1;      /* p_27 */

            seq[i] = pi; /* start with the non-homogeneous term */

            /* Add 17 * a(i-k) for k = 1 … min(i,26) */
            idx_t kmax = (i < 26) ? (idx_t)i : 26;
            for (idx_t k = 1; k <= kmax; ++k) {
                seq[i] += seq[i - k] * (seq_t)17;
            }

            /* Subtract 153 * a(i-27) for i ≥ 27 */
            if (i >= 27) {
                seq[i] -= seq[i - 27] * (seq_t)153;
            }
        }
        seq_len = n + 1;
    }
    return seq[n];
}
