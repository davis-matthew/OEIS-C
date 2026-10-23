/*
 * A135840  A135839 * A000012 as infinite lower triangular matrices.
 * https://oeis.org/A135840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(r,j) where: - r is the smallest integer satisfying
 *            r(r+1)/2 ≥ n - j = n − (r−1)r/2 (the column within row r) T(r,1)
 *            = ⌊(r+2)/2⌋, T(r,r) = 1, otherwise T(r,j) = ⌊(r−j+2)/2⌋. This is
 *            the row‑major flattening of the lower triangular matrix defined
 *            in the OEIS entry
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline idx_t row_for_n(idx_t n) {
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 < n)
            low = mid;
        else
            high = mid;
    }
    return high;
}

seq_t a(idx_t n) {
    // n ≥ 1
    idx_t r = row_for_n(n);
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;  // elements before row r
    idx_t j = n - prev;
    if (j == 1) {
        return (seq_t)((r + 2) / 2);
    } else if (j == r) {
        return (seq_t)1;
    } else {
        return (seq_t)((r - j + 2) / 2);
    }
}
