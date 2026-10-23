/*
 * A028222  Expansion of 1/((1-7x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A028222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)= sum_{k=1}^4 c_k * r_k^n, where r = {7,8,10,11}. Equivalent
 *            linear recurrence: a(0)=1, a(1)=36, a(2)=815, a(3)=14850,
 *            a(n)=36*a(n-1)-481*a(n-2)+2826*a(n-3)-6160*a(n-4) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values */
    const seq_t a0 = 1;
    const seq_t a1 = 36;
    const seq_t a2 = 815;
    const seq_t a3 = 14850;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* iterate recurrence */
    seq_t prev0 = a0;   /* a(n-4)   */
    seq_t prev1 = a1;   /* a(n-3)   */
    seq_t prev2 = a2;   /* a(n-2)   */
    seq_t prev3 = a3;   /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute: a(n) = 36*a(n-1) - 481*a(n-2) + 2826*a(n-3) - 6160*a(n-4) */
        seq_t term1 = prev3 * 36;          /* 36*a(n-1)      */
        seq_t term2 = prev2 * 481;         /* 481*a(n-2)     */
        seq_t term3 = prev1 * 2826;        /* 2826*a(n-3)    */
        seq_t term4 = prev0 * 6160;        /* 6160*a(n-4)    */

        /* use rearranged form to avoid unsigned underflow:
           (36*a(n-1)+2826*a(n-3)) - (481*a(n-2)+6160*a(n-4))
           which is always non-negative for all n≥4.           */
        seq_t expr1 = term1 + term3;       /* 36*a(n-1)+2826*a(n-3) */
        seq_t expr2 = term2 + term4;       /* 481*a(n-2)+6160*a(n-4) */

        seq_t new_val = expr1 - expr2;     /* a(n)            */

        /* shift for next iteration */
        prev0 = prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = new_val;
    }

    return prev3;   /* a(n) */
}
