/*
 * A094437  Triangular array T(n,k) = Fibonacci(k+2)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Fibonacci(k + 2) * binomial(r, k) where r is the row
 *            index such that r(r + 1)/2 ≤ n < (r + 1)(r + 2)/2 and k = n -
 *            r(r + 1)/2. This yields the term of the triangular array T(r,k)
 *            = Fibonacci(k + 2) * C(r,k) flattened row‑by‑row
 * Range:     u8 n <= 31, u16 n <= 98, u32 n <= 338, u64 n <= 1250,
 *            u128 n <= 4614, big any n
 * Validated: standard, pass, 81 terms through n = 5106 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    /* Find the row r and column k for the given index n. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        r++;
    r--;                                  /* now r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute the triangular array term T(r,k). */
    seq_t fib_val = fib(k + 2);                 /* Fibonacci(k + 2) */
    seq_t binom   = binomial(r, k);             /* C(r,k) */
    return fib_val * binom;
}
