/*
 * A093833  3^n-Jacobsthal(n).
 * https://oeis.org/A093833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n - (2^n - (-1)^n)/3 (equivalently a(n) = 3^n - ((2^n +
 *            (-1)^(n+1))/3))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n iteratively */
    seq_t pow2_val = 1;
    seq_t pow3_val = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow2_val *= (seq_t)2;
        pow3_val *= (seq_t)3;
    }

    /* Jacobsthal number J(n) = (2^n - (-1)^n)/3 */
    seq_t jacob;
    if ((n & 1u) == 0)          /* even n: (-1)^n = +1 */
        jacob = (pow2_val - (seq_t)1) / (seq_t)3;
    else                        /* odd n:  (-1)^n = -1 */
        jacob = (pow2_val + (seq_t)1) / (seq_t)3;

    /* a(n) = 3^n - J(n) */
    return pow3_val - jacob;
}
