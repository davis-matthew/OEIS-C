/*
 * A144152  Triangle read by rows: A128174 * X; X = an infinite lower
 *          triangular matrix with a shifted Fibonacci sequence: (1, 1, 1, 2,
 *          3, 5, 8, ...) in the main diagonal and the rest zeros.
 * https://oeis.org/A144152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = 0 if parity of row r and column c differ, otherwise a(n)
 *            = h[c] where h[1]=h[2]=h[3]=1 and h[k]=h[k-1]+h[k-2] for k>=4.
 *            Here r and c are the row and column indices of n when the
 *            triangle (1; 0,1; 1,0,1; 0,1,0,1; ...) is read row by row
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r containing n */
    idx_t r = 1;
    idx_t cum = 1; /* cumulative entries up to current row r */
    while (cum < n) {
        r++;
        cum += r;
    }
    /* column index within the row */
    idx_t prev = cum - r; /* sum of entries before this row */
    idx_t c = n - prev;

    /* parity check: odd rows have 1's at odd columns; even rows at even columns */
    if ((r & 1) != (c & 1)) {
        return 0;
    }

    /* compute the shifted Fibonacci-like sequence h[c] */
    if (c <= 3) {
        return 1;
    }
    seq_t prev2 = 1; /* h[2] */
    seq_t prev1 = 1; /* h[3] */
    seq_t curr = 0;
    for (idx_t i = 4; i <= c; ++i) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
