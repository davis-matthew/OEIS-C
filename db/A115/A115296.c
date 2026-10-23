/*
 * A115296  Skew version of correlation triangle for constant sequence 1.
 * https://oeis.org/A115296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flatten triangle T(n,k) for the skew correlation triangle
 *            where T(n,k) = 0 if 2k < n, = min(k,n-k,2k-n)+1 otherwise. a(n)
 *            is the n-th element in row-major order over all rows n>=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --- compute a(n) ---------------------------------------------- */
seq_t a(idx_t n) {
    /* Handle the first term directly. */
    if (n == 0) return (seq_t)1;

    /* Find the row index 'row' such that
       f(row) = row*(row+1)/2 <= n < f(row+1).  */
    idx_t low = 0;
    idx_t high = n;                      /* for n>0, f(n) > n always */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t row = low;
    unsigned __int128 prev_count = (unsigned __int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)prev_count;   /* column within the row */

    /* Compute T(row, col) = 0 if 2*col < row else min(col,row-col,2*col-row)+1. */
    unsigned __int128 double_col = (unsigned __int128)col << 1;   /* 2*col */
    if (double_col < row) return (seq_t)0;

    idx_t m1 = col;
    idx_t m2 = row - col;               /* row - col   >= 0  */
    idx_t m3 = (idx_t)(double_col - row); /* 2*col - row <= col */

    idx_t m = m1;
    if (m2 < m) m = m2;
    if (m3 < m) m = m3;

    return (seq_t)(m + 1);
}
