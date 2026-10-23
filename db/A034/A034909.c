/*
 * A034909  One third of octo-factorial numbers.
 * https://oeis.org/A034909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (∏_{j=1..n} (8*j-5)) / 3
 * Equivalent: 3*a(n) = (8*n-5)!!, the product of the arithmetic progression 3,11,19,...
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)                /* sequence starts at index 1 */
        return 0;
    seq_t prod = 1;
    for (idx_t j = 1; j <= n; ++j) {
        /* compute the j‑th factor 8*j-5 */
        seq_t factor = (seq_t)8 * j - 5;
        prod *= factor;
    }
    /* The product is always divisible by 3; division is exact. */
    return prod / 3;
}
