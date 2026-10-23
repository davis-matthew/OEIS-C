/*
 * A123580  The Kruskal-Macaulay function M_4(n).
 * https://oeis.org/A123580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..4} C(n_k-1, k-1) where the unique decomposition
 *            n = Σ_{k=1..t} C(n_k, k) (n_4 > n_3 > n_2 > n_1 ≥ 1) is the
 *            expansion in the combinatorial number system
 * Range:     u8 n <= 10, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t rem = (seq_t)n;
    seq_t res = 0;
    idx_t prev = (idx_t)-1;          /* allows choosing any n_4 initially */

    for (int k = 4; k >= 1; --k) {
        if (rem == 0) break;
        if (prev <= (idx_t)k) continue;

        idx_t lo = (idx_t)k;
        idx_t hi = prev - 1;         /* candidates for n_k are < prev */
        idx_t best = 0;

        /* binary search for the largest c_k with C(c_k, k) ≤ rem */
        while (lo <= hi) {
            idx_t mid = lo + ((hi - lo) >> 1);
            seq_t val = binomial((idx_t)mid, (idx_t)k);
            if (val <= rem) {
                best = mid;
                if (mid == hi) break;
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                if (mid == lo) break;
                hi = mid - 1;
            }
        }

        if (best >= (idx_t)k) {
            seq_t sub = binomial((idx_t)best, (idx_t)k);
            seq_t add = binomial((idx_t)(best - 1), (idx_t)(k - 1));
            rem -= sub;
            res += add;
            prev = best;
        }
    }
    return res;
}
