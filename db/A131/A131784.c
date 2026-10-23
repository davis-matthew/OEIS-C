/*
 * A131784  Triangle read by rows: (A004736 + A002260 - I) * A000012.
 * https://oeis.org/A131784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A131784: Triangle read by rows
 *  a(n) = ((i - j + 1) * (i + 1)) - 1
 *  where i is the smallest integer such that i(i+1)/2 >= n,
 *  and j = n - i(i-1)/2 is the column index (1‑based) within that row.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; n>=1 for the sequence */
    idx_t i = 1;
    idx_t tri = 1;                 /* triangular number t(i) = i(i+1)/2 */

    /* Find the row i such that t(i) >= n */
    while (tri < n) {
        ++i;
        tri += i;                 /* update to t(i) = i(i+1)/2 */
    }

    idx_t prev_tri = tri - i;     /* t(i-1) = i(i-1)/2 */
    idx_t j = n - prev_tri;       /* column index (1‑based) */

    seq_t row = (seq_t)i;
    seq_t col = (seq_t)j;
    return ((row - col + 1) * (row + 1)) - 1;
}
