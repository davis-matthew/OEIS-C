/*
 * A037622  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 8 + d_n, where the digits d_n repeat 2,0,3 (d_1
 *            = 2, d_2 = 0, d_3 = 3, d_4 = 2, …)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is unused */
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = i % 3;
        seq_t d = (r == 1) ? 2
               : (r == 2) ? 0
               : 3;
        val = val * 8 + d;
    }
    return val;
}
