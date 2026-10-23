/*
 * A134350  Row sums of triangle A134349.
 * https://oeis.org/A134350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 2*3^n - 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t pow2 = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
        pow2 *= (seq_t)2;

    /* compute 3^n */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    return pow2 + ( (seq_t)2 * pow3 ) - (seq_t)3;
}
