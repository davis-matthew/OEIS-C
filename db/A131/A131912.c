/*
 * A131912  Row sums of triangle A131911.
 * https://oeis.org/A131912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (22*n - 16)/4, if n is even; a(n) = (22*n - 18)/4, if n
 *            is odd
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = (22*n - 17 + (-1)^n) / 4 */
seq_t a(idx_t n)
{
    /* Working with idx_t prevents overflow on smaller seq_t types */
    idx_t m = n;
    idx_t num = (idx_t)22 * m;
    if ((m & 1) == 0)   /* even n */
        num -= (idx_t)16;
    else                /* odd n */
        num -= (idx_t)18;
    return (seq_t)(num / (idx_t)4);
}
