/*
 * A209614  G.f.: Sum_{n>=1} Fibonacci(n^3)*x^(n^3).
 * https://oeis.org/A209614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) if n is a perfect cube, else 0
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 63, u64 n <= 124,
 *            u128 n <= 215, big any n
 * Validated: standard, pass, 90 terms through n = 4051 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

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

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the n-th term of OEIS sequence A209614 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1, guard */

    /* Find floor cube root */
    idx_t k = int_cube_root(n);

    /* Check if k^3 == n (using 128‑bit to avoid overflow) */
    if ((unsigned __int128)k * k * k == n)
        return fib(n);            /* Fibonacci(n) where n=k^3 */

    return (seq_t)0;                /* not a perfect cube */
}
