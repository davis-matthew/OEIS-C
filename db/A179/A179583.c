/*
 * A179583  Number of permutations of 1..n+8 with deficiencies #{i : p(i) < i}
 *          exceeding exceedances #{i : p(i) > i} by n or more.
 * https://oeis.org/A179583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 23, u128 n <= 30,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179583: 
   a(n) = 4^(n+8) + 3^(n+6)*(n-11)*(n+8)/2
          + 2^(n+1)*(n+7)*(n+8)*(n^2-53*n-210)/3
          + (n+6)*(n+7)*(n+8)*(n^3-42*n^2+41*n+1860)/720 */
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* power 4^(n+8) */
    seq_t term1 = seq_pow((seq_t)4, n + (idx_t)8);

    /* term 2: 3^(n+6)*(n-11)*(n+8)/2, sign depends on (n-11)*(n+8) */
    int64_t poly2 = (int64_t)n - 11;
    poly2 *= (int64_t)n + 8;
    seq_t abs_poly2 = poly2 >= 0 ? (seq_t)poly2 : (seq_t)(-poly2);
    seq_t term2 = seq_pow((seq_t)3, n + (idx_t)6);
    term2 = term2 * abs_poly2 / (seq_t)2;

    /* term 3: 2^(n+1)*(n+7)*(n+8)*(n^2-53*n-210)/3, sign depends on the polynomial */
    int64_t poly3 = (int64_t)n * (int64_t)n - 53 * (int64_t)n - 210;
    seq_t abs_poly3 = poly3 >= 0 ? (seq_t)poly3 : (seq_t)(-poly3);
    seq_t term3 = seq_pow((seq_t)2, n + (idx_t)1);
    term3 = term3 * (seq_t)(n + 7) * (seq_t)(n + 8) * abs_poly3 / (seq_t)3;

    /* term 4: (n+6)*(n+7)*(n+8)*(n^3-42*n^2+41*n+1860)/720,
       sign depends on the cubic polynomial */
    int64_t poly4 = (int64_t)n * (int64_t)n * (int64_t)n
                    - 42 * (int64_t)n * (int64_t)n
                    + 41 * (int64_t)n + 1860;
    seq_t abs_poly4 = poly4 >= 0 ? (seq_t)poly4 : (seq_t)(-poly4);
    seq_t term4 = (seq_t)(n + 6) * (seq_t)(n + 7) * (seq_t)(n + 8)
                  * abs_poly4 / (seq_t)720;

    /* accumulate the signed contributions */
    seq_t result = term1;
    if (poly2 >= 0) result += term2; else result -= term2;

    if (poly3 >= 0) result += term3; else result -= term3;

    if (poly4 >= 0) result += term4; else result -= term4;

    return result;
}
