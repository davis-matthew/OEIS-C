/*
 * A212513  Number of (w,x,y,z) with all terms in {1,...,n} and w<=2x and
 *          y<=3z.
 * https://oeis.org/A212513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{x=1}^{n} min(n,2x)) * (∑_{z=1}^{n} min(n,3z))
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0)=0, a(1)=1, a(2)=16, a(3)=72, a(4)=210, a(5)=483, ...    */
#include <stdint.h>  /* ensure stdint types are available */

/* Compute A212513: number of quadruples (w,x,y,z) with 1≤w,x,y,z≤n,
   w ≤ 2x and y ≤ 3z.  The function works for any build that defines
   seq_t and idx_t.  */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic, but keep small intermediate
       calculations in idx_t to avoid overflow on very large n.  */
    seq_t sn = (seq_t)n;             /* n in seq_t arithmetic */
    idx_t m0 = n / 2;                /* floor(n/2)  */
    idx_t k0 = n / 3;                /* floor(n/3)  */
    seq_t m   = (seq_t)m0;           /* m as seq_t */
    seq_t k   = (seq_t)k0;           /* k as seq_t */

    /* Sum_{x=1..n} min(n,2x)  */
    seq_t sum_w = m*(m+1) + (sn - m)*sn;

    /* Sum_{z=1..n} min(n,3z)  */
    seq_t sum_y = 3 * (k*(k+1)/2) + (sn - k)*sn;

    return sum_w * sum_y;
}
