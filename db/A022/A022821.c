/*
 * A022821  [ (n+1)/(n-1) ] + [ (n+2)/(n-2) ] + ... + [ (2n-1)/1 ].
 * https://oeis.org/A022821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} floor((n+k)/(n-k))
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n < 2 the definition is not in the official range; return 0. */
    if (n < 2)
        return (seq_t)0;

    seq_t result = (seq_t)0;
    seq_t sn = (seq_t)n;

    /* Loop over k = 1 .. n-1 (idx_t is unsigned, so the loop terminates). */
    for (idx_t k = 1; k < n; ++k) {
        seq_t num = sn + (seq_t)k;   /* numerator (n + k) */
        seq_t den = sn - (seq_t)k;   /* denominator (n - k), always > 0 */
        result += num / den;         /* integer division gives floor */
    }

    return result;
}
