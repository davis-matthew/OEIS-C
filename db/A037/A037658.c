/*
 * A037658  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2.
 * https://oeis.org/A037658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + digit_{n-1}, with a(0)=0 and digits repeat
 *            [3,0,2]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int pattern[3] = {3, 0, 2};
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int digit = pattern[(i - 1) % 3];
        val = val * 9 + (seq_t)digit;
    }
    return val;
}
