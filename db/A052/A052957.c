/*
 * A052957  Expansion of 2*(1-x-x^2)/((1-2*x)*(1-2*x^2)).
 * https://oeis.org/A052957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052957: a(n) = 2^n for odd n; a(n) = 2^n + 2^(n/2) for even n */
#include <stdint.h>

static seq_t pow_two(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t two_n = pow_two(n);
    if (n & 1) return two_n;                /* odd n */
    return two_n + pow_two(n >> 1);          /* even n : add 2^(n/2) */
}
