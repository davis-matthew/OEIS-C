/*
 * A171476  a(n) = 6*a(n-1) - 8*a(n-2) for n > 1, a(0)=1, a(1)=6.
 * https://oeis.org/A171476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* Compute 2^n by repeated doubling */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n *= (seq_t)2;

    /* 2^(n+1) is just one more doubling */
    seq_t pow2_n_plus1 = pow2_n * (seq_t)2;

    /* a(n) = 2^(2n+1) - 2^n = 2^n * 2^(n+1) - 2^n */
    return pow2_n_plus1 * pow2_n - pow2_n;
}
