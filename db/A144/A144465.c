/*
 * A144465  a(n) = 5^n - 2^(n - 1) for n > 0; a(0) = 1.
 * https://oeis.org/A144465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 2^(n-1) for n > 0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef NO_EXTERN
extern seq_t a(idx_t n);
#endif

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Compute 5^n */
    seq_t five_pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        five_pow *= (seq_t)5;

    /* Compute 2^(n-1) */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 0; i < n-1; ++i)
        two_pow *= (seq_t)2;

    /* Return the difference */
    return five_pow - two_pow;
}
