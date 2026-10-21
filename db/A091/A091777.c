/*
 * A091777  Product of the numbers from (n-1)^2+1 to n^2.
 * https://oeis.org/A091777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of integers from (n-1)^2+1 to n^2 inclusive
 *            (equivalently, a(n) = (n^2)! / ((n-1)^2)! )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for lower indices, though unused by the spec */
        return (seq_t)1;

    idx_t n1 = n - 1;                 /* (n-1) safely fits in idx_t   */
    idx_t start = n1 * n1 + 1;        /* first factor in the product   */
    idx_t end   = n * n;              /* last factor in the product    */

    seq_t result = (seq_t)1;
    for (idx_t i = start; i <= end; ++i)
        result *= (seq_t)i;

    return result;
}
