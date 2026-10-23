/*
 * A199207  a(n) = 7*4^n+1.
 * https://oeis.org/A199207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*4^n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: use exponentiation by squaring on unsigned 4^n */

#include <stdint.h>

/* Compute 4^exp using exponentiation by squaring */
static seq_t pow4_u64(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    while (exp != 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p = pow4_u64(n);
    p *= (seq_t)7;
    p += (seq_t)1;
    return p;
}
