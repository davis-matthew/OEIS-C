/*
 * A060589  a(n) = 2*(2^n-1)*3^(n-1).
 * https://oeis.org/A060589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (2^n - 1) * 3^(n-1) (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^n */
    seq_t pow2n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2n <<= 1;          /* multiply by 2 */

    /* Compute 3^(n-1) */
    seq_t pow3n1 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow3n1 *= (seq_t)3;

    /* a(n) = 2 * (pow2n - 1) * pow3n1 */
    seq_t result = pow2n - (seq_t)1;
    result *= (seq_t)2;
    result *= pow3n1;

    return result;
}
