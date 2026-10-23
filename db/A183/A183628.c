/*
 * A183628  Number of (n+1)X6 0..2 arrays with every 2x2 subblock summing to
 *          4.
 * https://oeis.org/A183628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 124*2^n + 3*3^n + 602, n ≥ 1
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the power of an integer base to an exponent using binary exponentiation.
   All intermediate operations use the type seq_t. */
static seq_t power_int(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow2n = power_int((seq_t)2, n);
    seq_t pow3n = power_int((seq_t)3, n);
    /* Compute 124*pow2n + 3*pow3n + 602 */
    return (seq_t)124 * pow2n + (seq_t)3 * pow3n + (seq_t)602;
}
