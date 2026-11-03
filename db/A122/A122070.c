/*
 * A122070  Triangle given by T(n,k) = Fibonacci(n+k+1)*binomial(n,k) for
 *          0<=k<=n.
 * https://oeis.org/A122070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fib(r + k + 1) * binomial(r, k), where r = (sqrt(8*n + 1)
 *            - 1) / 2 (integer division) and k = n - r*(r + 1)/2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 161, u64 n <= 577,
 *            u128 n <= 2113, big any n
 * Validated: standard, pass, 71 terms through n = 4838 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

seq_t a(idx_t n) {
    /* Compute r = floor((sqrt(8*n + 1) - 1)/2) */
    seq_t val = (seq_t)8 * (seq_t)n + 1;          /* 8*n + 1 as seq_t */
    seq_t s   = floor_sqrt(val);                  /* floor(sqrt(8*n + 1)) */
    idx_t r   = (idx_t)((s - (seq_t)1) / (seq_t)2);

    /* Compute k = n - offset(row r) */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Fib(r + k + 1) */
    idx_t fib_n = r + k + 1;
    seq_t f = fib(fib_n);

    /* binomial(r, k) */
    seq_t b = binomial(r, k);

    return f * b;
}
