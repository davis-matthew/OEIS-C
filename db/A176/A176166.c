/*
 * A176166  a(n) is the maximum exponent in the prime factorization of the
 *          n-th triangular number.
 * https://oeis.org/A176166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of n(n+1)/2
 *            (i.e. of the n-th triangular number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_exp_from_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/max_exp_from_factor.h"

seq_t a(idx_t n) {
    /* Decompose T_n = n(n+1)/2 as described:
       If n is even:  T_n = (n/2)*(n+1)
       If n is odd:   T_n = n*((n+1)/2)           */
    idx_t x1, x2;
    if ((n & 1) == 0) {            /* n even */
        x1 = n >> 1;               /* n/2 */
        x2 = n + 1;
    } else {                       /* n odd */
        x1 = n;
        x2 = (n + 1) >> 1;         /* (n+1)/2 */
    }

    seq_t max1 = max_exp_from_factor(x1);
    seq_t max2 = max_exp_from_factor(x2);
    return (max1 > max2) ? max1 : max2;
}
