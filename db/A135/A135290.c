/*
 * A135290  Row 11 of A038207.
 * https://oeis.org/A135290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = C(11,n) * 2^(11-n)
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n>11 the binomial coefficient is zero */
    if (n > 11) return 0;

    /* Compute C(11,n) by multiplicative formula */
    idx_t k = n;
    if (k > 11 - k) k = 11 - k;          /* use the smaller of n and 11-n */

    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* binom = binom * (11 - k + i) / i;  using seq_t arithmetic */
        binom *= (seq_t)(11 - k + i);
        binom /= (seq_t)i;
    }

    /* Multiply by 2^(11-n) */
    seq_t res = binom;
    for (idx_t i = 0; i < 11 - n; ++i)
        res *= (seq_t)2;

    return res;
}
