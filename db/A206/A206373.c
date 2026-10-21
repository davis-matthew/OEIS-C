/*
 * A206373  a(n) = (14*4^n + 1)/3.
 * https://oeis.org/A206373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*4^n + 1)/3 * Recurrence: a(0) = 5, a(n+1) = 4*a(n) -
 *            1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 5;
    for (idx_t i = 0; i < n; ++i) {
        value = value * 4 - 1;
    }
    return value;
}
