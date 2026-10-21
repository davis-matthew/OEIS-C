/*
 * A212509  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and
 *          y<=3z.
 * https://oeis.org/A212509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (sum_{x=1}^n min(n, 2x-1)) * (sum_{z=1}^n min(n, 3z))
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = [ t^2 + (n-t)*n ] * [ 3*s*(s+1)/2 + (n-s)*n ] */
/*          where t = floor((n+1)/2), s = floor(n/3) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to seq_t for arithmetic */
    seq_t nt = (seq_t)n;

    /* t = floor((n+1)/2)  ,  s = floor(n/3)   (all integer idx_t arithmetic) */
    idx_t t_idx = (n + 1) / 2;
    idx_t s_idx = n / 3;
    seq_t t = (seq_t)t_idx;
    seq_t s = (seq_t)s_idx;

    /* S1 = t^2 + (n-t)*n */
    seq_t S1 = t * t + (nt - t) * nt;

    /* S2 = 3*s*(s+1)/2 + (n-s)*n */
    seq_t tmp = (seq_t)3 * s * (s + 1) / 2;
    seq_t S2 = tmp + (nt - s) * nt;

    return S1 * S2;
}
