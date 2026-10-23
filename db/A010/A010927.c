/*
 * A010927  Binomial coefficient C(11,n).
 * https://oeis.org/A010927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(11,n)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 11) return (seq_t)0;

    /* use symmetry: C(11,n) = C(11,11-n) */
    idx_t k = n;
    if (k > 11 - k) k = 11 - k;

    seq_t result = 1;
    idx_t start = 11 - k + 1;          /* first factor in numerator   */
    for (idx_t i = 0; i < k; ++i) {
        result = result * (seq_t)(start + i);   /* multiply numerator factor  */
        result = result / (seq_t)(i + 1);       /* divide by denominator       */
    }
    return result;
}
