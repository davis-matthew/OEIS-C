/*
 * A056453  Number of palindromes of length n using exactly two different
 *          symbols.
 * https://oeis.org/A056453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor((n + 1) / 2) - 2 for n >= 1
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for 0, return 0 for safety */

    idx_t exp = (n + 1) / 2;             /* floor((n+1)/2) */
    seq_t pow2 = (seq_t)1;

    /* compute 2^exp safely, using repeated multiplication */
    for (idx_t i = 0; i < exp; ++i)
    {
        /* multiply by 2 each iteration */
        pow2 = pow2 << 1;                 /* for built-in types shift works;
                                            for arbitrary-precision integer
                                            types this is well-defined */
    }

    /* pow2 is 2^exp; subtract 2 */
    return pow2 - (seq_t)2;
}
