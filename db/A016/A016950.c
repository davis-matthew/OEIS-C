/*
 * A016950  a(n) = (6*n + 3)^6.
 * https://oeis.org/A016950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 270, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * n + (seq_t)3;
    seq_t res  = base * base;      /* base^2 */
    res        = res * base;       /* base^3 */
    res        = res * res;        /* base^6 */
    return res;
}
