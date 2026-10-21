/*
 * A084628  a(n) = floor(binomial(n+7,7)/binomial(n+3,3)).
 * https://oeis.org/A084628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+4)(n+5)(n+6)(n+7)/840)
 * Range:     u8 none, u16 n <= 10, u32 n <= 292, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n + 4;          /* n+4 */
    prod *= (seq_t)n + 5;               /* (n+4)(n+5) */
    prod *= (seq_t)n + 6;               /* (n+4)(n+5)(n+6) */
    prod *= (seq_t)n + 7;               /* (n+4)(n+5)(n+6)(n+7) */
    return prod / 840;                  /* floor division */
}
