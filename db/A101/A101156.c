/*
 * A101156  a(n) = 2*Fibonacci(n) + 8*Fibonacci(n-5).
 * https://oeis.org/A101156
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n) + 8*Fibonacci(n-5)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen.h"
#include <stddef.h>

static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn_next)
{
    seq_t a = 0;          /* F(0) */
    seq_t b = 1;          /* F(1) */
    idx_t bits = bitlen(n);
    while (bits--) {
        seq_t d = a * ((b << 1) - a);
        seq_t e = a * a + b * b;
        if ((n >> bits) & 1) {
            a = e;
            b = d + e;
        } else {
            a = d;
            b = e;
        }
    }
    *fn = a;
    if (fn_next) *fn_next = b;
}

seq_t a(idx_t n)
{
    /* n >= 5 by definition of the sequence offset */
    seq_t Fn, Fn5, dummy;
    fib_pair(n, &Fn, &dummy);
    fib_pair(n-5, &Fn5, &dummy);
    return (seq_t)2 * Fn + (seq_t)8 * Fn5;
}
