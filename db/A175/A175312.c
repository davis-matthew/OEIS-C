/*
 * A175312  Maximum value on Lower Shuffle Part of Kimberling's Expulsion
 *          Array (A035486).
 * https://oeis.org/A175312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3*(n - floor(log₂((n+2)/3))) -
 *            floor((n+2)/2^floor(log₂((n+2)/3)))
 * Range:     u8 n <= 90, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is irrelevant because the offset is 1 */
    if (n == 0)
        return 0;

    /* Compute λ(n) = floor(log₂((n+2)/3)) without helpers */
    idx_t v = (n + 2) / 3;
    idx_t lambda = 0;
    while (v > 1) {
        v >>= 1;
        lambda++;
    }

    /* Perform all arithmetic in seq_t to avoid overflow */
    seq_t term1 = 1;
    seq_t term2 = (seq_t)3 * ((seq_t)n - (seq_t)lambda);
    seq_t term3 = (seq_t)((n + 2) >> lambda);   /* floor((n+2)/2^lambda) */

    return term1 + term2 - term3;
}
