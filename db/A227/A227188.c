/*
 * A227188  Square array A(n,k) read by antidiagonals: the one-based bit-index
 *          where the (k+1)-st run in the binary expansion of n ends, as read
 *          from the least significant end.
 * https://oeis.org/A227188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = value obtained by reading square array A(n,k) where
 *            A(n,k) = 1‑based bit-index of the (k+1)‑st run end in the binary
 *            representation of n (bits counted from the least significant
 *            bit), taken along antidiagonals. The function maps n to the
 *            corresponding (row,col) position, then scans the bin...
 * Range:     u8 n <= 8282, u16 n <= 8282, u32 n <= 8282, u64 n <= 8282,
 *            u128 n <= 8282, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal `d` such that n is on that diagonal.
       The number of terms up to and including diagonal d is
       (d+1)(d+2)/2. */
    idx_t d = 0;
    while (1) {
        __int128 tri = (__int128)(d + 1) * (d + 2) / 2;
        if ((__int128)n < tri)
            break;
        ++d;
    }

    /* Compute the position inside the diagonal */
    __int128 prev = (__int128)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)prev;      /* row index i */
    idx_t row    = offset;
    idx_t col    = d - offset;           /* column index j (k) */

    /* By definition A(0,k)=0 for all k. */
    if (row == 0)
        return 0;

    /* Scan binary representation of `row` to record run endpoints
       (zero‑based bit indices).  A 64‑bit integer can have at most 64
       runs; we store them in a small fixed array. */
    idx_t ends[64];
    size_t cnt = 0;

    idx_t m = row;
    idx_t pos = 0;
    int prev_bit = m & 1;                /* bit at current position */

    /* Process all bits except the most significant; each time the bit
       changes we record the end of the run that just finished. */
    while (m > 1) {
        m >>= 1;
        pos++;
        int bit = m & 1;
        if (bit != prev_bit) {
            ends[cnt++] = pos - 1;       /* end of previous run */
            prev_bit = bit;
        }
    }
    /* Record the end of the last run (at the most significant bit). */
    ends[cnt++] = pos;                    /* last run ends at bit 'pos' */

    /* `col` is the (k)-th entry in the row (zero‑based).  If there are
       fewer than (col+1) runs the value is 0. */
    if ((idx_t)col >= cnt)
        return 0;

    /* Return the one‑based bit index of the (col+1)-st run end. */
    return (seq_t)(ends[col] + 1);
}
