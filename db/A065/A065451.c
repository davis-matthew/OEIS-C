/*
 * A065451  a(n) = Fibonacci(phi(n)), a(0) = 0.
 * https://oeis.org/A065451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A000010(n)), a(0)=0
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 52, u64 n <= 96, u128 n <= 190,
 *            big any n
 * Validated: standard, pass, 64 terms through n = 936 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"

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
    if (n == (idx_t)0)
        return 0;

    /* φ(n) always fits in idx_t for n ≤ 10 000 */
    seq_t phi_val = totient(n);
    idx_t fib_idx = (idx_t)phi_val;
    return fib(fib_idx);
}
