/*
 * A019523  Concatenation of Fibonacci(1) through Fibonacci(n).
 * https://oeis.org/A019523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 86 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of Fibonacci(1) through Fibonacci(n)
   Formula: a(n) = concat(Fib(1), Fib(2), …, Fib(n))
   where Fib(1)=1, Fib(2)=1, Fib(k)=Fib(k-1)+Fib(k-2) for k>2. */

/*  Dependencies provided by the build environment:
    - seq_t fib(idx_t n);
    - seq_t pow10(idx_t e);
    - int digit_len(seq_t x);
*/

#include <stddef.h>
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

seq_t a(idx_t n)
{
    if (n < 1) {
        /* The sequence is defined for n >= 1.
           Returning 0 for invalid input is harmless. */
        return (seq_t)0;
    }

    /* Start with a(1) = Fib(1) = 1 */
    seq_t result = fib((idx_t)1);

    if (n == 1) {
        return result;
    }

    /* Prepare Fibonacci numbers Fib(1)=1, Fib(2)=1 */
    seq_t f_prev = fib((idx_t)1);
    seq_t f_curr = fib((idx_t)2);

    for (idx_t i = 2; i <= n; ++i) {
        /* Current Fibonacci number to append */
        seq_t f = f_curr;

        /* Number of decimal digits of f */
        int d = num_digits(f);

        /* Result = result * 10^d + f */
        seq_t pow = pow10((idx_t)d);
        result = result * pow + f;

        /* Advance Fibonacci sequence for the next iteration */
        seq_t tmp = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = tmp;
    }

    return result;
}
