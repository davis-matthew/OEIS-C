/*
 * A076939  Duplicate of A016116.
 * https://oeis.org/A076939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^floor((n-1)/2)
 * Range:     i8 n <= 14, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)2;   /* multiply by 2 for each step */
    return result;
}
