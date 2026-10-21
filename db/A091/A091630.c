/*
 * A091630  a(n) = A091628(n) + product of digits of A091628(n).
 * https://oeis.org/A091630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2*(10^(n+1) - 1)/9 + 3*2^n + 1 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the term A091630 for index n >= 1. */
#include <stdint.h>

static seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n < 1 */

    /* 10^(n+1) and 2^n */
    seq_t ten_pow = pow_uint((seq_t)10, n + (idx_t)1);
    seq_t two_pow = pow_uint((seq_t)2, n);

    /* Term 1: 2*(10^(n+1) - 1)/9, computed safely as ((t-1)/9)*2 */
    seq_t term1 = ((ten_pow - (seq_t)1) / (seq_t)9) * (seq_t)2;

    /* Term 2: 3*2^n */
    seq_t term2 = (seq_t)3 * two_pow;

    return term1 + term2 + (seq_t)1;
}
