/*
 * A141213  Defining A to be the interior angle of a regular polygon, the
 *          number of constructible regular polygons such that A is in a field
 *          extension = degree 2^n, starting with n=0. This is also the number
 *          of values of x such that phi(x)/2 = 2^n (where phi is Euler's phi
 *          function), also starting with n=0.
 * https://oeis.org/A141213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+3 for n ≤ 31, otherwise a(n) = 34
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Because n is unsigned, compare directly with 31. */
    if (n <= 31)
        return (seq_t)n + 3;
    else
        return 34;
}
