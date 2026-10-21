/*
 * A227154  Product of digits+1 of n in factorial base.
 * https://oeis.org/A227154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   product of (digit+1) over the digits of n in factorial base
 * Range:     u8 n <= 334, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t prod = 1;
    idx_t i = 1;
    while (n > 0) {
        idx_t digit = n % (i + 1);
        prod *= (seq_t)(digit + 1);
        n /= (i + 1);
        ++i;
    }
    return prod;
}
