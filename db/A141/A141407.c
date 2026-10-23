/*
 * A141407  a(n) = binomial(n+7,7)*6^n.
 * https://oeis.org/A141407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*6^n
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* binomial part will start at 1 */

    /* compute binomial(n+7, 7) multiplicatively: product_{k=1}^7 (n+7-(k-1))/k */
    for (idx_t k = 1; k <= 7; ++k) {
        seq_t numerator = (seq_t)(n + 7 - (k - 1));
        res = res * numerator;     /* multiply by the current numerator */
        res = res / (seq_t)k;       /* exact division by the current denominator */
    }

    /* compute 6^n */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow6 *= (seq_t)6;
    }

    /* combine the two parts */
    res = res * pow6;
    return res;
}
