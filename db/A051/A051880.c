/*
 * A051880  a(n) = binomial(n+4,4)*(2*n+1).
 * https://oeis.org/A051880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+4,4)*(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 136, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t binom = 1;

    /* Compute C(n+4,4) multiplicatively to keep intermediate sizes small. */
    for (idx_t i = 1; i <= 4; ++i) {
        binom *= (nn + 5 - i);  /* multiply by (n+4, n+3, n+2, n+1) in order */
        binom /= i;            /* divide by 1,2,3,4 respectively */
    }

    seq_t twonp1 = (seq_t)n * 2 + 1;   /* (2*n + 1) as seq_t */
    return binom * twonp1;
}
