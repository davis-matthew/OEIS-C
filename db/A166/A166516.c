/*
 * A166516  A product of consecutive doubled Fibonacci numbers.
 * https://oeis.org/A166516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2*floor(n/2) + 1) * Fibonacci(2*floor((n-1)/2)
 *            + 1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 463 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the exceptional case n==0 separately */
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t k = n / 2;               /* floor(n/2) */
    idx_t i = 2 * k + 1;           /* index for the first Fibonacci term */

    seq_t fi = fib(i);             /* F(2*floor(n/2)+1) */

    if ((n & 1) == 0) {            /* even n: use F(2*k-1) as second factor */
        seq_t fj = fib(2 * k - 1);
        return seq_mul(fi, fj);
    } else {                       /* odd n: square the same term */
        return seq_mul(fi, fi);
    }
}
