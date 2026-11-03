/*
 * A130633  Additive persistence of Fibonacci numbers.
 * https://oeis.org/A130633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A031286(A000045(n)) – additive persistence of the n‑th
 *            Fibonacci number
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 48, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   digit_sum, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

seq_t a(idx_t n)
{
    seq_t f = fib(n);          /* F(n) */
    seq_t p = 0;               /* persistence counter */

    /* Repeatedly sum the decimal digits until a single digit remains */
    while (num_digits(f) > 1) {
        f = digit_sum(f);
        p++;
    }
    return p;
}
