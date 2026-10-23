/*
 * A016103  Expansion of 1/((1-4x)(1-5x)(1-6x)).
 * https://oeis.org/A016103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3+2*n) + 2^(1+n) * 3^(2+n) - 5^(2+n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types */

static seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t exp1 = 3 + 2 * n;
    idx_t exp2 = 1 + n;
    idx_t exp3 = 2 + n;

    seq_t t1 = pow_uint(2, exp1);                      /* 2^(3+2*n) */
    seq_t t2 = pow_uint(2, exp2) * pow_uint(3, exp3); /* 2^(1+n) * 3^(2+n) */
    seq_t t3 = pow_uint(5, exp3);                      /* 5^(2+n) */

    return t1 + t2 - t3;
}
