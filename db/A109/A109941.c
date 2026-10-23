/*
 * A109941  Sum of all n-digit multiples of n.
 * https://oeis.org/A109941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of all n-digit multiples of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute 10^(n-1) and 10^n */
    seq_t p10_n_minus1 = pow10(n - 1);   /* 10^(n-1)  */
    seq_t p10_n       = pow10(n);       /* 10^n      */

    /* Convert n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* First k such that k*n has n digits:
       ceil(10^(n-1) / n) */
    seq_t k1 = (p10_n_minus1 + sn - 1) / sn;

    /* Last k such that k*n has n digits:
       floor((10^n - 1) / n) */
    seq_t k2 = (p10_n - 1) / sn;

    /* Number of terms */
    seq_t count = (k2 >= k1) ? (k2 - k1 + 1) : 0;
    if (count == 0) return 0;

    /* Sum of k from k1 to k2: (k1 + k2) * count / 2 */
    seq_t sum_k = (k1 + k2) * count;
    sum_k /= 2;          /* product is always even */

    /* Final sum of multiples */
    seq_t result = sum_k * sn;
    return result;
}
