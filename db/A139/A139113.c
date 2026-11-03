/*
 * A139113  Concatenation of n and n-th Fibonacci number.
 * https://oeis.org/A139113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(n, Fib(n)) (decimal concatenation of n and the
 *            n-th Fibonacci number)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the n-th term of OEIS sequence A139113: concatenation of n and Fib(n) */
seq_t a(idx_t n)
{
    /* Compute the n-th Fibonacci number */
    seq_t fn = fib(n);

    /* Number of decimal digits in fn */
    int d = num_digits(fn);

    /* 10^d  (big integer) */
    seq_t pow10_d = pow10(d);

    /* Concatenate: n * 10^d + fn */
    return (seq_t)n * pow10_d + fn;
}
