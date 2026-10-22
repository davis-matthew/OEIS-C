/*
 * A006262  Product k^(2^(k-2)), k = 2..n.
 * https://oeis.org/A006262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product k^(2^(k-2)), k = 2..n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

static seq_t power_seq(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 2; k <= n; k++) {
        seq_t exp = A001832_pow2(k - 2);
        seq_t term = power_seq((seq_t)k, exp);
        result *= term;
    }
    return result;
}
