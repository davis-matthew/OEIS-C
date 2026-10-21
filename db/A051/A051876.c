/*
 * A051876  24-gonal numbers: a(n) = n*(11*n-10).
 * https://oeis.org/A051876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (11*n - 10) (24-gonal numbers)
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts. */
    seq_t tn = (seq_t)n;
    /* Compute 11*n in seq_t arithmetic to avoid overflow of idx_t. */
    seq_t eleven_n = (seq_t)11 * tn;
    /* a(n) = n * (11*n - 10) */
    return tn * (eleven_n - (seq_t)10);
}
