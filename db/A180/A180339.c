/*
 * A180339  Triangle by rows, A137710 * a diagonalized variant of A001906.
 * https://oeis.org/A180339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   * Let r be the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 *            and * let c = n − r(r+1)/2 (so 0 ≤ c ≤ r). Then * * a(n) = 2^r
 *            if c = 0 * a(n) = fib(2c) if c = r > 0 * a(n) = fib(2c) *
 *            2^(r−c−1) otherwise * * (Here fib(k) denotes the k‑th Fibonacci
 *            number, with * fib(0) = fib(2) = 1 for the purpo...
 * Range:     u8 n <= 34, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* find r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t c = n - r * (r + 1) / 2;

    /* diagonal value q = fib(2c) for c >= 2, otherwise 1 */
    seq_t q = 1;
    if (c >= 2)
        q = fib(2 * c);

    if (c == 0) {
        /* first element of the row */
        return pow2(r);
    } else if (r == c) {
        /* diagonal element */
        return q;
    } else {
        /* off-diagonal element */
        return q * pow2(r - c - 1);
    }
}
