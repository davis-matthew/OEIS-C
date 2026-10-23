/*
 * A112035  a(n) = Sum_{k=0..n} k*C(n,k)^2*C(n+k,k)^3, where C := binomial.
 * https://oeis.org/A112035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k*C(n,k)^2*C(n+k,k)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    /* accumulative sum of the sequence term */
    seq_t sum = 0;

    /* C(n,0) and C(n+0,0) */
    seq_t curC = 1;  /* current binomial C(n,k)   */
    seq_t d    = 1;  /* current binomial C(n+k,k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term contributes only for k>0 (factor k) */
        if (k != 0) {
            seq_t curC2 = mul128(curC, curC);     /* C(n,k)^2      */
            seq_t d2    = mul128(d, d);           /* C(n+k,k)^2    */
            seq_t d3    = mul128(d2, d);          /* C(n+k,k)^3    */
            seq_t term  = mul128(curC2, d3);      /* square * cube  */
            term = mul128(term, (seq_t)k);        /* multiply by k  */
            sum += term;                          /* add to result  */
        }
        /* terminate when k == n (no next binomial needed) */
        if (k == n)
            break;

        /* update binomials for next k+1:
           C(n,k+1)   = C(n,k)*(n-k)/(k+1)
           C(n+k+1,k+1) = C(n+k,k)*(n+k+1)/(k+1) */
        curC = mul128(curC, (seq_t)(n - k)) / (seq_t)(k + 1);
        d    = mul128(d,    (seq_t)(n + k + 1)) / (seq_t)(k + 1);
    }

    return sum;
}
