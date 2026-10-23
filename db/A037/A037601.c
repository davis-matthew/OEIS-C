/*
 * A037601  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + d(n), with a(0)=0, and d(n) repeats 1,3,0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = (i - 1) % 3;
        if (r == 0) {
            val = val * 8 + 1;
        } else if (r == 1) {
            val = val * 8 + 3;
        } else {
            val = val * 8; /* r == 2, digit is 0 */
        }
    }
    return val;
}
