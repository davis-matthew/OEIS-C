/*
 * A008823  Expansion of (1+2*x^3+x^5)/((1-x)^2*(1-x^5)).
 * https://oeis.org/A008823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2*n+3)/5) + 1
 * Range:     u8 n <= 10, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                  /* promote index to seq_t */
    seq_t prod = nn * (nn * 2 + 3);       /* n*(2*n+3) */
    return prod / 5 + 1;                 /* floor division + 1 */
}
