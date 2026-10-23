/*
 * A008647  Expansion of g.f.: (1+x^9)/((1-x^4)*(1-x^6)).
 * https://oeis.org/A008647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n/2) + floor(n/3) + floor(n/4) - n + 1
 * Range:     u8 n <= 311, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t t2 = sn / 2;            /* floor(n/2) */
    seq_t t3 = sn / 3;            /* floor(n/3) */
    seq_t t4 = sn / 4;            /* floor(n/4) */
    seq_t result = t2 + t3 + t4 - sn + 1;
    return result;
}
