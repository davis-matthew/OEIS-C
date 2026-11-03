/*
 * A187107  Number of nontrivial compositions of differential operations and
 *          directional derivative of the n-th order on the space R^9.
 * https://oeis.org/A187107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000045(n) + 7
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A000045(n) is the nth Fibonacci number with F(1)=1, F(2)=1 */
#include <stddef.h>
#include <stdbool.h>

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
    if (n < 1) return (seq_t)0;               /* behavior below offset is unspecified */
    return fib(n) + (seq_t)7;
}
