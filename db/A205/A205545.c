/*
 * A205545  Symmetric matrix by antidiagonals: C(max(3i,3j),min(3i,3j)), i>=0,
 *          j>=0.
 * https://oeis.org/A205545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A205545
   A205545: Symmetric matrix by antidiagonals: C(max(3i,3j),min(3i,3j)), i>=0, j>=0.
   Sequence terms are obtained by flattening the infinite matrix along antidiagonals
   (s = i + j) and listing entries in the natural order of each diagonal
   (i = 0 … s).  This function computes a(n) for any n >= 1. */
#include "../utils/binom.h"

static idx_t _diag_index(idx_t n) {
    /* Find the smallest t such that (t+1)(t+2)/2 >= n.
       t is the diagonal sum index (i+j). */
    idx_t low = 0, high = 1;
    while ((high + 1) * (high + 2) / 2 < n)
        high <<= 1;          /* double until upper bound is reached */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid + 1) * (mid + 2) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* offset is 1, n>=1 */
    idx_t t = _diag_index(n);     /* diagonal index (sum i+j) */

    /* total elements before this diagonal */
    idx_t total_before = t * (t + 1) / 2;
    idx_t k = n - total_before - 1;   /* zero‑based position within diagonal */

    idx_t i = k;          /* i varies from 0 to t */
    idx_t j = t - k;      /* j = t - i */

    idx_t three_i = 3 * i;
    idx_t three_j = 3 * j;

    idx_t a = three_i > three_j ? three_i : three_j;
    idx_t b = three_i < three_j ? three_i : three_j;

    return binom(a, b);
}
