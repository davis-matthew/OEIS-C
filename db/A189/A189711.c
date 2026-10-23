/*
 * A189711  Number of non-monotonic functions from [k] to [n-k].
 * https://oeis.org/A189711
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = T(n,k) for k = 3 + (n-5th element in row n) The sequence
 *            enumerates all triangle entries T(n,k) for n≥5, k=3..n‑2 in
 *            order of increasing n and then increasing k. T(n,k) is (n-k)^k -
 *            2*C(n-1,k) + (n-k)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 117, u64 n <= 344,
 *            u128 n <= 980, big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Off‑set of 5: a(5) is the first triangle element (row 5, k=3). */
    if (n < 5) return 0;                 /* behaviour below offset irrelevant */

    idx_t pos = n - 5;                    /* 0‑based position in the enumeration */

    /* Find the row R such that C(R-4) > pos, where C(m) = m*(m+1)/2
       gives the cumulative number of elements up to row R. */
    idx_t low = 5, high = 5;
    /* Grow high until cumulative > pos */
    while (((uint64_t)(high - 4) * (high - 3)) / 2 <= pos) high++;

    /* Binary search for the smallest R with cumulative > pos */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t cum = ((uint64_t)(mid - 4) * (mid - 3)) / 2;
        if (cum <= pos)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t R = low;                       /* row index (≥5) */

    /* Number of entries before this row */
    idx_t cum_before = ((uint64_t)(R - 5) * (R - 4)) / 2;
    idx_t offset_in_row = pos - cum_before;  /* 0‑based index inside the row */

    idx_t k = 3 + offset_in_row;          /* column index */

    seq_t nk = (seq_t)(R - k);            /* n - k (always ≥ 2) */

    /* Compute T(n,k) = (n-k)^k - 2*C(n-1,k) + (n-k) */
    seq_t term1 = pow_seq(nk, (int)k);
    seq_t term2 = binomial(R - 1, k);
    term2 = term2 + term2;                /* multiply by 2 */
    seq_t term3 = nk;

    return term1 - term2 + term3;
}
