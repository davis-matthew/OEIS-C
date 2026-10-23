/*
 * A135471  a(n) = Sum{i=1..n} ( i*2^(i-1) ) - ( A002024(n)*(A002024(n)+1)/2 -
 *          n ) * 2^(A002024(n)-1).
 * https://oeis.org/A135471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Corrected formula:  
 * a(n) = Sum_{i=1..n} i*2^{i-1}
 *        - ( k*(k+1)/2 - n ) * 2^{k-1},
 * where k is the smallest integer with k*(k+1)/2 >= n. 
 * For n>=1 this reproduces the OEIS sequence A135471. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1 */

    /* S(n) = (n-1)*2^n + 1 */
    seq_t two_pow_n = pow2(n);
    seq_t S = ((seq_t)(n - 1)) * two_pow_n + 1;

    /* Find k = ceil( (sqrt(8n+1)-1)/2 ), i.e. smallest k with k*(k+1)/2 >= n */
    idx_t k = 0;
    idx_t tri = 0;
    while (tri < n) {
        k++;
        tri += k;                      /* tri = k*(k+1)/2 at loop end */
    }

    seq_t K = (seq_t)k;
    seq_t tri_k = K * (K + 1) / 2;      /* k*(k+1)/2 */
    seq_t two_pow_k_minus1 = pow2(k - 1);/* 2^{k-1} */

    seq_t term = (tri_k - (seq_t)n) * two_pow_k_minus1;
    seq_t result = S - term;

    return result;
}
