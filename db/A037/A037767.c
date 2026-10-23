/*
 * A037767  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2,1.
 * https://oeis.org/A037767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the number whose base‑6 representation consists of the
 *            first n digits of the periodic sequence 3,0,2,1, with the first
 *            digit being the most significant. That is, a(1)=3, a(2)=30_6=18,
 *            a(3)=302_6=110, …
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const uint8_t period[4] = {3, 0, 2, 1};
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * 6 + (seq_t)period[(i - 1) & 3];
    }
    return result;
}
