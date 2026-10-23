/*
 * A037547  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-6 number formed by the first n digits of the
 *            infinite periodic sequence 1,2,2,1,2,2,1,2,2,... (Offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i;
    for (i = 1; i <= n; ++i) {
        seq_t digit = ((i - 1) % 3 == 0) ? 1 : 2;
        res = res * 6 + digit;
    }
    return res;
}
