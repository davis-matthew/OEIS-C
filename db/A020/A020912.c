/*
 * A020912  Number of terms in base 4 representation of n-th Fibonacci number.
 * https://oeis.org/A020912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log_4(F(n))) + 1, where F(n) is the n‑th Fibonacci
 *            number
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}
#include <stdbool.h>

/* Number of digits in the base‑4 representation of the n‑th Fibonacci number */
seq_t a(idx_t n)
{
    /* The Fibonacci function is guaranteed to be defined for n ≥ 1. */
    seq_t f   = fib(n);                     /* F(n) as a seq_t */
    unsigned bits = bit_length(f);        /* number of bits in F(n) */
    /* digits = floor((bits‑1)/2) + 1 */
    seq_t digits = ((seq_t)(bits - 1) >> 1) + 1;
    return digits;
}
