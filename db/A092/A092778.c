/*
 * A092778  Concatenate pairs of successive Fibonacci numbers.
 * https://oeis.org/A092778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000045(n)*10^(A055642(A000045(n+1))) + A000045(n+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   fib_pair, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fib_pair.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);                 /* F(n), F(n+1) */
    seq_t mult = pow10((idx_t)num_digits(fn1));
    return fn * mult + fn1;
}
