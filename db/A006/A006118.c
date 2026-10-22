/*
 * A006118  Sum of Gaussian binomial coefficients [ n,k ] for q=4.
 * https://oeis.org/A006118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + (4^(n-1)-1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2 = 1;   /* a(i-2) */
    seq_t prev1 = 2;   /* a(i-1) */
    seq_t p = 4;       /* 4^(i-1) for i = 2 */
    idx_t i;

    if (n == 0) return prev2;
    if (n == 1) return prev1;

    for (i = 2; i <= n; i++) {
        seq_t cur = 2 * prev1 + (p - 1) * prev2;
        prev2 = prev1;
        prev1 = cur;
        p *= 4;
    }
    return prev1;
}
