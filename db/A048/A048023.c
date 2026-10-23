/*
 * A048023  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1 , 3t≤⌊(n‑2)/3⌋ , 2t≤n‑⌊(n‑2)/3⌋}
 *            C(k,3t)·C(n‑k,2t), where k = ⌊(n‑2)/3⌋
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* k = floor((n-2)/3) */
    idx_t k = (n > 2 ? (n - 2) / 3 : 0);
    idx_t nk = n - k;           /* remaining elements */

    /* Determine the maximum t allowed by the two constraints:
       3t ≤ k  and  2t ≤ n-k. */
    idx_t tmax1 = k / 3;        /* from 3t ≤ k */
    idx_t tmax2 = nk / 2;       /* from 2t ≤ n-k */
    idx_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;

    for (idx_t t = 1; t <= tmax; ++t) {
        idx_t r1 = 3 * t;   /* elements chosen from first k numbers */
        idx_t r2 = 2 * t;   /* elements chosen from the rest */

        seq_t c1 = binomial(k, r1);
        seq_t c2 = binomial(nk, r2);
        sum += c1 * c2;
    }

    return sum;
}
