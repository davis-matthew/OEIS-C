/*
 * A037568  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret the first n digits of the infinite * periodic
 *            sequence 2,1,2 as a base‑3 number. That is, * a(1) = 2, a(2) = 7
 *            (21₃), a(3) = 23 (212₃), etc
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char pattern[3] = {2, 1, 2};
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
        val = val * 3 + pattern[i % 3];
    return val;
}
