/*
 * A168428  a(n) = 4^n mod 10.
 * https://oeis.org/A168428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n mod 10
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* 4^0 = 1 */
        return (seq_t)1;
    /* For n ≥ 1, 4^n mod 10 alternates: 4, 6, 4, 6, … */
    return (n & 1) ? (seq_t)4 : (seq_t)6;
}
