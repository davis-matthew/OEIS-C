/*
 * A070691  (Sum of digits of n)^n.
 * https://oeis.org/A070691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum of decimal digits of n)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t.h"

seq_t a(idx_t n) {
    /* Compute sum of digits of n */
    uint64_t t = n;
    unsigned sum = 0u;
    while (t) {
        sum += (unsigned)(t % 10u);
        t /= 10u;
    }
    /* Raise the digit sum to the power n */
    return pow_seq_t((seq_t)sum, n);
}
