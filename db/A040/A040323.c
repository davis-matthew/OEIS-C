/*
 * A040323  Continued fraction for sqrt(342).
 * https://oeis.org/A040323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 18 ; for n ≥ 1, a(n) alternates 2 and 36 : a(n) = 2 if n
 *            is odd, otherwise 36
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;
    /* all other terms are periodic with period 2 */
    return (n & 1) ? (seq_t)2 : (seq_t)36;
}
