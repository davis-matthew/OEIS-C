/*
 * A037791  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3; a(n)=9*a(n-1)+digit(n) where digit(n) cycles 3,2,0,1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[4] = {3, 2, 0, 1};
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 9 + digits[(i - 1) % 4];
    }
    return res;
}
