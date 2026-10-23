/*
 * A123576  The Kruskal-Macaulay function L_4(n).
 * https://oeis.org/A123576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   L_4(n): write n = Σ C(n_k,k) with decreasing k (k=4,3,2,1). Then
 *            L_4(n) = Σ C(n_k,k+1) (C(k+1)=C(n_k,k+1))
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t rem = n;
    idx_t prev_k = n + 1;          /* limit for the next binomial */

    for (int r = 4; r >= 1 && rem > 0; r--) {
        idx_t limit = prev_k - 1;      /* binomial indices must be < prev_k */
        if (limit < (idx_t)r) continue; /* cannot use this r, skip */

        idx_t k = 0;
        for (idx_t cand = limit; cand >= (idx_t)r; cand--) {
            seq_t val = binomial(cand, (idx_t)r);
            if (val <= (seq_t)rem) {   /* found the largest possible k */
                k = cand;
                break;
            }
        }

        if (k == 0) /* Should never happen because rem > 0 */
            continue;

        rem -= (idx_t)binomial(k, (idx_t)r);            /* subtract contribution */
        sum += binomial(k, (idx_t)(r + 1));            /* add to L_4 */
        prev_k = k;                                   /* enforce decreasing k */
    }

    return sum;
}
