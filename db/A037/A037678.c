/*
 * A037678  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-8 number whose digits are 1,0,2,3 repeating
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t d;
        idx_t r = i & 3ULL;   /* i % 4, but faster with bitwise AND since 4 is 2^2 */
        if (r == 1)       d = 1;
        else if (r == 2)  d = 0;
        else if (r == 3)  d = 2;
        else              d = 3;  /* r == 0 (i % 4 == 0) */
        res = res * 8 + d;
    }
    return res;
}
