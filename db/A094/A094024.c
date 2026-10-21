/*
 * A094024  Alternating 1 with one less than the powers of 2.
 * https://oeis.org/A094024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1-(-1)^n) * 2^floor(n/2) + (-1)^n Equivalently: a(n) = 1
 *            if n is even a(n) = 2^((n+1)/2) - 1 if n is odd
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t power_of_two(idx_t bits) {
    seq_t result = (seq_t)1;
    /* Use multiplication to avoid UB on large shift counts */
    for (idx_t i = 0; i < bits; ++i) {
        result = result * (seq_t)2;
    }
    return result;
}

seq_t a(idx_t n) {
    if ((n & (idx_t)1) == 0)       /* n is even: a(n) = 1 */
        return (seq_t)1;
    else {                         /* n is odd: a(n) = 2^((n+1)/2) - 1 */
        idx_t exp = (n + (idx_t)1) >> 1;   /* (n+1)/2 */
        return power_of_two(exp) - (seq_t)1;
    }
}
