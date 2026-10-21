/*
 * A074155  Group the natural numbers so that the product of members of a
 *          group is a multiple of the sum:
 *          (1),(2,3,4,5,6),(7,8,9),(10,11,12),(13,14,15),(16,17,18),(19,20,21),(22,23,24),....
 *          This is the sequence of the ratio of product /sum.
 * https://oeis.org/A074155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(3*n-2) for n>2; a(1) = 1; a(2) = 36
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)36;
    /* compute using seq_t arithmetic to avoid overflow issues */
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)3 * nn - (seq_t)2);
}
