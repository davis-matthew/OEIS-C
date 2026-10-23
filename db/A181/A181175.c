/*
 * A181175  The "Fi1" sums of the powers-of-2 triangle A000079.
 * https://oeis.org/A181175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2; a(2*n+1) = 2^(2*n+1) * a(2*n) a(2*n+2) =
 *            2^(2*n+2) * ((4^(n+2)-1)/(4^(n+1)-1)) * a(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The function below implements this recurrence directly.) */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Handle the first two trivial values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* a_even holds a(2*k) for the current k, starting with a(0)=1. */
    seq_t a_even = (seq_t)1;

    for (idx_t k = 0; ; ++k)
    {
        /* Compute a(2*k+1) if the index is within the range. */
        idx_t idx_odd = 2 * k + 1;
        seq_t a_odd = 0;

        if (idx_odd <= n)
        {
            a_odd = pow2(idx_odd) * a_even;
            if (idx_odd == n) return a_odd;
        }

        /* Next even index is 2*k+2. */
        idx_t idx_even = idx_odd + 1;
        if (idx_even > n) break;

        /* Recurrence for a(2*k+2).  Multiply in stages to
           keep intermediate values within range for small n. */
        seq_t t_num = pow4((idx_t)(k + 2));
        t_num -= (seq_t)1;                         /* 4^(k+2)-1 */
        seq_t t_den = pow4((idx_t)(k + 1));
        t_den -= (seq_t)1;                         /* 4^(k+1)-1 */

        seq_t temp = a_odd;
        temp *= pow2(idx_even);                    /* 2^(2*k+2) */
        temp *= t_num;
        temp /= t_den;

        a_even = temp; /* Now a_even holds a(2*(k+1)) */

        if (idx_even == n) return a_even;
    }

    /* If the loop exits without returning we reached a negative index
       or n is below the offset.  This case should not occur for n >= 0. */
    return (seq_t)0;
}
