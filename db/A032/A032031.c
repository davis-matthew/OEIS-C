/*
 * A032031  Triple factorial numbers: (3n)!!! = 3^n*n!.
 * https://oeis.org/A032031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3^n * n! (a(0) = 1, a(n) = 3 * n * a(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence grows extremely fast; an iterative product is
       sufficient and works for all supported seq_t types. */
    seq_t result = 1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        result *= (seq_t)3 * (seq_t)i;   /* 3 * i */
    }
    return result;
}
