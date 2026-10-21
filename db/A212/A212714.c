/*
 * A212714  Number of (w,x,y,z) with all terms in {1,...,n} and |w-x| >= w +
 *          |y-z|.
 * https://oeis.org/A212714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^4/8)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This formula holds for all n >= 0 (offset 0). */
seq_t a(idx_t n)
{
    /* Compute n^4 as seq_t intermediate to avoid overflow of idx_t */
    seq_t t = (seq_t)n;
    t *= n;          /* t = n^2   */
    t *= n;          /* t = n^3   */
    t *= n;          /* t = n^4   */
    return t / 8;    /* floor division */
}
