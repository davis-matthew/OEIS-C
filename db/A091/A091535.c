/*
 * A091535  First column (k=2) of array A091534 ((5,2)-Stirling2).
 * https://oeis.org/A091535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (3*n-1)*(3*n-2)*a(n-1) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n<1, return 0 just in case */
    }
    seq_t result = 1;
    for (idx_t k = 2; k <= n; ++k) {
        /* multiply by (3*k-1) and (3*k-2) in a single step */
        result *= (seq_t)(3*k - 1);
        result *= (seq_t)(3*k - 2);
    }
    return result;
}
