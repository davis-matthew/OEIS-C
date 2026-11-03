/*
 * A119587  2^n + 1 - 2*Fibonacci(n+1).
 * https://oeis.org/A119587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 1 - 2*Fib(n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 956 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast exponentiation and Fibonacci from the utils library. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t pow_val  = pow2(n);          /* 2^n   */
    seq_t fib_val  = fib(n + 1);       /* Fib(n+1) */
    seq_t twofib   = mul_small(fib_val, 2); /* 2*Fib(n+1) */

    seq_t result = pow_val + (seq_t)1 - twofib;
    return result;
}
