/*
 * A060869  Number of n X n matrices over GF(4) with rank 1.
 * https://oeis.org/A060869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/3 * (4^n - 1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    /* Compute (4^n - 1) */
    seq_t t = pow4 - (seq_t)1;

    /* Result: (t * t) / 3 */
    return (t * t) / (seq_t)3;
}
