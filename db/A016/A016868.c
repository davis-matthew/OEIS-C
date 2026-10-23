/*
 * A016868  a(n) = (5n+1)^8.
 * https://oeis.org/A016868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (5*n+1)^8 using only integer arithmetic */
    seq_t base = (seq_t)5 * (seq_t)n + 1;
    seq_t pow  = base * base;   /* base^2   */
    pow        = pow * pow;     /* base^4   */
    pow        = pow * pow;     /* base^8   */
    return pow;
}
