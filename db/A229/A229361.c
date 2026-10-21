/*
 * A229361  Total sum of n-th powers of parts in all partitions of 10.
 * https://oeis.org/A229361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 97 + 41*2^n + 21*3^n + 13*4^n + 8*5^n + 5*6^n + 3*7^n +
 *            2*8^n + 9^n + 10^n
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    static const seq_t coeff[10] = { 97, 41, 21, 13, 8, 5, 3, 2, 1, 1 };
    seq_t total = 0;
    for (idx_t k = 1; k <= 10; ++k) {
        seq_t term = coeff[k - 1] * pow_seq((seq_t)k, n);
        total += term;
    }
    return total;
}
