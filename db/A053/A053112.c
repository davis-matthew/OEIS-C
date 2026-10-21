/*
 * A053112  Expansion of (-1 + 1/(1-9*x)^9)/(81*x); related to A053108.
 * https://oeis.org/A053112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n) = 9^(n-1) * binomial(n+9, 8) for n>=1
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute binomial(n+9, 8) multiplicatively */
    seq_t binom = (seq_t)1;
    for (idx_t k = 1; k <= 8; ++k) {
        binom *= (n + 10 - k);   /* numerator: n+9, n+8, ..., n+2 */
        binom /= k;              /* exact division */
    }

    /* compute 9^(n-1) */
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow9 *= (seq_t)9;
    }

    return pow9 * binom;
}
