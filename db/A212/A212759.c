/*
 * A212759  Number of (w,x,y,z) with all terms in {0,...,n} and w, x, and y
 *          even.
 * https://oeis.org/A212759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/2)+1)^3*(n+1)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of even numbers in [0..n] */
    seq_t e = ((seq_t)n / 2) + 1;
    /* compute e^3 * (n+1) */
    seq_t ans = e * e * e;
    ans = ans * ((seq_t)n + 1);
    return ans;
}
