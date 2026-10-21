/*
 * A078836  a(n) = n*2^(n-6).
 * https://oeis.org/A078836
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^(n-6)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, return 0 (no defined value). */
    if (n < 6)
        return (seq_t)0;

    /* Compute 2^(n-6) by repeated multiplication. */
    seq_t pow2 = (seq_t)1;
    idx_t exp = n - 6;
    for (idx_t i = 0; i < exp; ++i)
        pow2 = pow2 * ((seq_t)2);

    /* Return n * 2^(n-6). */
    return (seq_t)n * pow2;
}
