/*
 * A100789  First differences of A000543.
 * https://oeis.org/A100789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/12)(2n+1)(2n^6+6n^5+11n^4+12n^3+25n^2+20n+12)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the generic integer type */
    seq_t N = (seq_t)n;

    /* Compute the polynomial P(n) = 2n^6+6n^5+11n^4+12n^3+25n^2+20n+12 using Horner's rule */
    seq_t P = (seq_t)2;
    P = P * N + (seq_t)6;   /* n^5 term */
    P = P * N + (seq_t)11;  /* n^4 term */
    P = P * N + (seq_t)12;  /* n^3 term */
    P = P * N + (seq_t)25;  /* n^2 term */
    P = P * N + (seq_t)20;  /* n   term */
    P = P * N + (seq_t)12;  /* constant term */

    /* Compute t1 = 2n + 1 */
    seq_t t1 = (seq_t)2 * N + (seq_t)1;

    /* Compute the result: a(n) = (t1 * P) / 12 */
    seq_t num = t1 * P;
    seq_t result = num / (seq_t)12;

    return result;
}
