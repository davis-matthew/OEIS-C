/*
 * A054609  a(n) = Sum_{d|10} phi(d)*n^(10/d).
 * https://oeis.org/A054609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 + n^5 + 4*n^2 + 4*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    /* n^10 */
    seq_t pow10 = sn;
    for (idx_t i = 1; i < 10; ++i) pow10 *= sn;
    /* n^5 */
    seq_t pow5  = sn;
    for (idx_t i = 1; i < 5; ++i) pow5 *= sn;
    /* 4*n^2 */
    seq_t n2 = sn * sn;
    n2 *= (seq_t)4;
    /* 4*n */
    seq_t n4 = sn * (seq_t)4;
    /* sum */
    return pow10 + pow5 + n2 + n4;
}
