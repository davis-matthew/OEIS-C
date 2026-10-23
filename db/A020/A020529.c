/*
 * A020529  4th Bernoulli polynomial evaluated at powers of 2 (multiplied by
 *          30).
 * https://oeis.org/A020529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 30*16^n - 60*8^n + 30*4^n - 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 14, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow2 *= 2;
    }

    /* powers of 2 for 4^n, 8^n, 16^n */
    seq_t pow4  = pow2 * pow2;     /* 2^(2n) */
    seq_t pow8  = pow4 * pow2;     /* 2^(3n) */
    seq_t pow16 = pow8 * pow2;     /* 2^(4n) */

    /* combine according to the closed form */
    seq_t result = 30 * pow16 - 60 * pow8 + 30 * pow4 - 1;
    return result;
}
