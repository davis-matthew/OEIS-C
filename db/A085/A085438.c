/*
 * A085438  a(n) = Sum_{i=1..n} binomial(i+1,2)^3.
 * https://oeis.org/A085438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} ((i*(i+1)/2)^3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 41, u64 n <= 1003,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = (seq_t)i * ((seq_t)i + 1) / 2; /* binomial (i+1,2) */
        seq_t c = t * t * t;                     /* cube of the binomial */
        sum += c;
    }
    return sum;
}
