/*
 * A162396  a(n) = 2*a(n-2) for n > 2; a(1) = 5, a(2) = 2.
 * https://oeis.org/A162396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n > 2, a(1) = 5, a(2) = 2. (Offset 1)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(n) = 2^(⌊n/2⌋) * (n odd?5:1). */

seq_t a(idx_t n)
{
    /*  n must be >= 1; values for n < 1 are undefined.  */
    /*  Compute 2^(floor(n/2))  */
    idx_t exp = n >> 1;          /* floor(n/2)   */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; ++i)
        pow2 *= (seq_t)2;        /* multiply by 2 */

    /*  Multiply by 5 if n is odd.  */
    if ((n & (idx_t)1u) != 0)
        return pow2 * (seq_t)5;
    else
        return pow2;
}
