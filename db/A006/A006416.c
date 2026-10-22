/*
 * A006416  Number of loopless rooted planar maps with 3 faces and n vertices
 *          and no isthmuses. Also a(n)=T(4,n-3), array T as in A049600.
 * https://oeis.org/A006416
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n+3, 3) - 3*(n+1)
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t a_val = n_s + 1;
    seq_t b_val = n_s + 2;
    seq_t c_val = n_s + 3;

    // Compute binomial(n+3, 3) = (n+3)*(n+2)*(n+1) / 6
    // Divide by 2 and 3 intelligently to avoid overflow
    if (a_val % 2 == 0) a_val /= 2;
    else if (b_val % 2 == 0) b_val /= 2;
    else c_val /= 2;

    if (a_val % 3 == 0) a_val /= 3;
    else if (b_val % 3 == 0) b_val /= 3;
    else c_val /= 3;

    seq_t binom = a_val * b_val * c_val;
    return binom - 3 * (n_s + 1);
}
