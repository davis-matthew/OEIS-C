/*
 * A086752  Number of n X n matrices over GF(3) with rank n-1.
 * https://oeis.org/A086752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3^n-1)/2) * ∏_{j=0..n-2} (3^n-3^j) ( OEIS A086752 )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;         /* a(1) = 1 */

    /* Compute 3^n */
    seq_t pow3n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3n *= 3;

    /* Compute the product ∏ (3^n - 3^j) for j = 0 .. n-2 */
    seq_t prod = 1;
    seq_t pow3j = 1;              /* 3^0 */
    for (idx_t j = 0; j <= n - 2; ++j) {
        prod *= (pow3n - pow3j);
        pow3j *= 3;               /* next power of 3 */
    }

    /* Multiply by (3^n - 1) / 2 */
    seq_t factor = (pow3n - 1) / 2;

    return prod * factor;
}
