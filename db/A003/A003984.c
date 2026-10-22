/*
 * A003984  Table of max(x,y), where (x,y) =
 *          (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),...
 * https://oeis.org/A003984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(x,y) = max(x,y) read by antidiagonals: with d the largest
 *            integer such that d*(d+1)/2 <= n, x = n - d*(d+1)/2, y = d - x,
 *            a(n) = max(x,y)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        d++;
    idx_t x = n - d * (d + 1) / 2;
    idx_t y = d - x;
    return (x > y) ? (seq_t)x : (seq_t)y;
}
