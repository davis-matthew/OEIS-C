/*
 * A116746  Number of permutations of length n which avoid the patterns 1243,
 *          4123, 4213.
 * https://oeis.org/A116746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 2*3^(n-1) - ((1+√2)^(n+2)+(1-√2)^(n+2))/4 + 1/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for out-of-range index */

    /* 2^n */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* 3^(n-1)  (for n==1 this is 3^0 = 1) */
    seq_t pow3 = seq_pow((seq_t)3, n - 1);

    /* 2*3^(n-1) = pow3 + pow3 */
    seq_t two_pow3 = pow3 + pow3;

    /* b = 2^n + 2*3^(n-1) */
    seq_t b = pow2 + two_pow3;

    /* Recurrence for t_k = (1+√2)^k + (1-√2)^k :
       t0 = 2, t1 = 2, t_k = 2*t_{k-1} + t_{k-2}  */
    seq_t t_prevprev = (seq_t)2;   /* t_{k-2}   */
    seq_t t_prev     = (seq_t)2;   /* t_{k-1}   */
    seq_t t_k        = t_prev;     /* placeholder for t_{k} */

    /* n+2 >= 3 for n >= 1, so iterate from i=2 to i=n+2 */
    for (idx_t i = 2; i <= n + 2; ++i) {
        t_k = ((seq_t)2 * t_prev) + t_prevprev;
        t_prevprev = t_prev;
        t_prev     = t_k;
    }
    /* t_k now holds t_{n+2} */

    /* Compute (4*b - t_{n+2} + 2)/4 */
    seq_t num = (b << 2) - t_k + (seq_t)2;
    seq_t ans = num >> 2;   /* division by 4 */

    return ans;
}
