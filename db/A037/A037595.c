/*
 * A037595  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*9 + digit_{n-1}, where digit_k = {1,0,3} repeating
 *            (0-indexed). a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[3] = {1, 0, 3};
    seq_t value = 0;
    for (idx_t i = 0; i < n; ++i) {
        value = value * 9 + digits[i % 3];
    }
    return value;
}
