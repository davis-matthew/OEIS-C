/*
 * A084857  Inverse binomial transform of n^2*3^(n-1).
 * https://oeis.org/A084857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n - 1)*2^(n - 2) for n >= 1; a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for all n >= 0. */

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly. */
    if (n == 0)
        return 0;

    /* For n = 1 the formula would involve 2^(-1).  We return the
       known value a(1) = 1 directly. */
    if (n == 1)
        return 1;

    /* Compute 2^(n-2) by successive multiplication.  This works for
       arbitrary-precision seq_t types as well as for fixed-width
       unsigned types. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 *= 2;

    /* Compute n*(3*n - 1) using seq_t arithmetic. */
    seq_t prod = (seq_t)n * (((seq_t)3 * n) - 1);

    /* Final result.  All intermediate values are computed with
       seq_t, leaving overflow to be handled by the underlying type. */
    return prod * pow2;
}
