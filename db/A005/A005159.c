/*
 * A005159  a(n) = 3^n*Catalan(n).
 * https://oeis.org/A005159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3^n*Catalan(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t pow3(idx_t n) {
    seq_t result = 1;
    seq_t base = 3;
    while (n > 0) {
        if (n & 1) result *= base;
        base *= base;
        n >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t binom_val = binom(2*n, n);
    seq_t catalan = binom_val / ((seq_t)(n + 1));
    seq_t power_3 = pow3(n);
    return power_3 * catalan;
}
