/*
 * A037690  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0,3.
 * https://oeis.org/A037690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 6 + digit_{n-1}; digit_i = {1, 2, 0, 3}[i mod
 *            4]; a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[4] = { (seq_t)1, (seq_t)2, (seq_t)0, (seq_t)3 };
    seq_t const base = (seq_t)6;
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * base + period[i & 3];
    return res;
}
