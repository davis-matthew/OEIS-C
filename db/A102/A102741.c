/*
 * A102741  a(n) = 3^4 * binomial(n+3, 4).
 * https://oeis.org/A102741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81 * binomial(n+3,4) = 81 * (n+3)(n+2)(n+1)n / 24
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 187, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);
    prod *= (nn + 2);
    prod *= (nn + 3);
    seq_t binom = prod / 24;          /* integer division, exact for n ≥ 1 */
    return binom * (seq_t)81;         /* 81 = 3^4 */
}
