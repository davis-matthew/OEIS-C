/*
 * A074544  a(n) = 2^n + 7^n + 8^n.
 * https://oeis.org/A074544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation that works for any integer type seq_t.
   For unsigned wrappers it uses modulo arithmetic. For arbitrary‑precision
   seq_t this gives the exact power. */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;                 /* seq_t literal 1 is fine */
    while (exp) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* compute 2^n, 7^n and 8^n and sum them */
    seq_t v2 = seq_pow((seq_t)2, n);
    seq_t v7 = seq_pow((seq_t)7, n);
    seq_t v8 = seq_pow((seq_t)8, n);
    return v2 + v7 + v8;
}
