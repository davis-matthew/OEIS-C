/*
 * A071307  1/2 number of n X n 0..8 matrices with MM' mod 9 = I.
 * https://oeis.org/A071307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 , n = 1 * a(n) = n! * 2^(n-1) * 3^(n^2 - 3*n + 3) , n
 *            >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              // undefined for offset < 1
    if (n == 1) return 1;

    /* factorial */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* 2^(n-1) */
    seq_t pow2 = pow_seq((seq_t)2, n - 1);

    /* 3^(n^2 - 3*n + 3) */
    idx_t exp3 = n * n - 3 * n + 3;   // does not overflow uint64_t for n <= 10000
    seq_t pow3 = pow_seq((seq_t)3, exp3);

    return fact * pow2 * pow3;
}
