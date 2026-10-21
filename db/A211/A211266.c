/*
 * A211266  Number of integer pairs (x,y) such that 0<x<y<=n and x*y<=2n.
 * https://oeis.org/A211266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with 0 < x < y ≤ n and x*y
 *            ≤ 2n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* 0 < x < y ≤ n  →  x ranges from 1 to n-1 */
    for (idx_t x = 1; x < n; ++x) {
        /* upper bound for y from x+1 to min(n, floor(2n/x)) */
        seq_t y_max = ((seq_t)2 * (seq_t)n) / (seq_t)x;
        if (y_max > n) y_max = n;
        if (y_max > x) total += y_max - x;
    }
    return total;
}
