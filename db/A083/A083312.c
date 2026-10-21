/*
 * A083312  Largest integer m such that 1+2+...+m divides n.
 * https://oeis.org/A083312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer m such that 1+2+...+m ( = m*(m+1)/2 )
 *            divides n
 * Range:     u8 n <= 1043, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Two times n for easier divisibility test:  m*(m+1) divides 2*n  */
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* Upper bound for m: floor((sqrt(1+8*n)-1)/2).  This is
       guaranteed to be >= the true answer for all n >= 1. */
    double d = sqrt( (double)n * 8.0 + 1.0 );
    idx_t max_m = (idx_t)((d - 1.0) / 2.0);

    /* Search from max_m downwards; stop at first divisor. */
    for (idx_t m = max_m; ; --m) {
        /* Compute product m*(m+1) as seq_t to avoid overflow in idx_t. */
        seq_t prod = (seq_t)m * (seq_t)(m + 1);
        if (two_n % prod == 0)
            return (seq_t)m;
        if (m == 1)   /* after testing m==1, there is no smaller m */
            break;
    }
    return (seq_t)1;   /* safety: at least m = 1 always works */
}
