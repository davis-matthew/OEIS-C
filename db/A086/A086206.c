/*
 * A086206  Number of n X n matrices with entries in {0,1} with no zero row
 *          and with zero main diagonal.
 * https://oeis.org/A086206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n-1)-1)^n (OEIS A086206)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is defined. */
    if (n == 0)
        return (seq_t)0;            /* value not defined for n=0, return 0 */

    /* Compute 2^(n-1) as a seq_t */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        two_pow *= (seq_t)2;          /* two_pow = 2^(n-1) */

    /* Compute x = 2^(n-1) - 1 */
    seq_t x = two_pow - (seq_t)1;

    /* Compute x^n by repeated multiplication (binary exponentiation not needed for small n) */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        result *= x;

    return result;
}
