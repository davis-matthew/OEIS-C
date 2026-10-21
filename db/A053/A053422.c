/*
 * A053422  n times (n 1's): a(n) = n*(10^n - 1)/9.
 * https://oeis.org/A053422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*(10^n - 1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute 10^n */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= (seq_t)10;

    /* Compute n * (10^n - 1) */
    seq_t prod = (seq_t)n * (pow10 - (seq_t)1);

    /* Divide by 9 (the result is always integer) */
    prod /= (seq_t)9;

    return prod;
}
