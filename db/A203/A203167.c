/*
 * A203167  (n-1)-st elementary symmetric function of the first n terms of
 *          (2,2,1,2,2,1,2,2,1,...)=(A130196 for n>0).
 * https://oeis.org/A203167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(c2-1)*(2*n - c2), where c2 = 2*(n/3) + (n%3)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    idx_t c2 = 2 * q + r;           /* number of 2's among the first n terms */

    /* compute 2^(c2-1) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 1; i < c2; ++i)
        pow *= (seq_t)2;

    /* compute (2*n - c2) as seq_t */
    seq_t factor = (seq_t)n;
    factor += factor;                 /* 2*n */
    factor -= (seq_t)c2;              /* 2*n - c2 */

    return pow * factor;
}
