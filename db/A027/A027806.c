/*
 * A027806  a(n) = 30*(n+1)*binomial(n+4,10).
 * https://oeis.org/A027806
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*(n+1)*binomial(n+4,10)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 22, u64 n <= 163, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 6; for smaller n we return 0 */
    if (n < 6) return 0;

    /* compute binomial(n+4,10) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 0; i < 10; i++) {
        /* multiply by the next factor (n-5 + i) and divide by (i+1) */
        binom = binom * (seq_t)(n - 5 + i);
        binom = binom / (i + 1);
    }

    /* a(n) = 30 * (n+1) * binom */
    seq_t res = binom * ((seq_t)(n + 1));
    res *= 30;
    return res;
}
