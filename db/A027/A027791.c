/*
 * A027791  a(n) = 5*(n+1)*binomial(n+3,6).
 * https://oeis.org/A027791
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)*binomial(n+3,6)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* undefined for n < 3 in this implementation */

    /* Compute binomial(n+3,6) multiplicatively.  */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        bin = bin * ((seq_t)(n + 4 - i));   /* multiply by (n+4-i) */
        bin = bin / i;                     /* exact division by i */
    }

    /* a(n) = 5*(n+1)*bin */
    return 5 * ((seq_t)(n + 1)) * bin;
}
