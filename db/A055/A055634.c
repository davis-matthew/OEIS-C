/*
 * A055634  2-adic factorial function.
 * https://oeis.org/A055634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (2*floor(n/2)+1)!!
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 20, i64 n <= 34, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of odd factors to multiply: (n+1)/2 */
    idx_t t = (n + 1) >> 1;
    seq_t prod = 1;
    for (idx_t k = 1; k <= t; ++k)
    {
        /* current odd factor */
        seq_t odd = (seq_t)(2 * k - 1);
        prod *= odd;
    }
    /* apply alternating sign: positive for even n, negative for odd n */
    if (n & 1)
        prod = -prod;
    return prod;
}
