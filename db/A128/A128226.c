/*
 * A128226  Triangle, A004736 * A127899 (unsigned).
 * https://oeis.org/A128226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*j + 1)*r - 2*j*j where r = smallest integer with
 *            r*(r+1)/2 ≥ n (row number) and j = n - r*(r-1)/2 (column number
 *            in that row)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t _find_row(idx_t n) {
    /* binary search for smallest r such that r*(r+1)/2 >= n */
    idx_t lo = 0, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n) hi = mid;
        else lo = mid;
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, but avoid UB */
    /* find row r */
    idx_t r = _find_row(n);
    /* offset of first element in this row (0‑based count of preceding rows) */
    idx_t offset0 = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
    idx_t j = n - offset0;                /* column number, 1‑based */
    /* compute the value using seq_t arithmetic */
    seq_t r_seq = (seq_t)r;
    seq_t j_seq = (seq_t)j;
    seq_t val   = (2 * j_seq + 1) * r_seq - 2 * j_seq * j_seq;
    return val;
}
