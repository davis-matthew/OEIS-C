/*
 * A136673  Triangle T(n,k) read by rows: T(n,k) = [x^k] x^n+x+1, 0 <= k <=
 *          max(n, 1).
 * https://oeis.org/A136673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, dumb, less
 * Formula:   a(n) is the coefficient of x^k in (x^n + x + 1) where k is the
 *            column index obtained by flattening the triangle T(n,k) = [x^k]
 *            (x^n + x + 1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline idx_t floor_isqrt_u64(idx_t x)
{
    /* Integer square‑root: returns ⌊√x⌋ using only integer arithmetic. */
    idx_t r = 0;
    idx_t b = ((idx_t)1) << 32;          /* 2^32 > √(2^64‑1) */
    while (b != 0) {
        idx_t mid = r + b;
        if (mid <= x / mid) {
            r = mid;
        }
        b >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Trivial first two terms. */
    if (n == 0u) return 2;
    if (n == 1u) return 1;

    /* For n ≥ 2 compute : row r and column k in the flattened triangle. */
    idx_t m = n - 2u;                            /* offset after the first row */
    idx_t disc = floor_isqrt_u64(9u + 8u * m);    /* ⌊√(9 + 8m)⌋ */
    idx_t r = (disc >= 3u ? (disc - 3u) / 2u : 0u) + 1u;  /* row index */

    /* Starting index of row r in the linear sequence. */
    idx_t start = 2u + ((r - 1u) * (r + 2u)) / 2u;

    idx_t k = n - start;                 /* column within the row (0‑based) */

    /* Compute the coefficient at position (r, k). */
    seq_t val = 0;
    if (k == 0u) ++val;                  /* constant term (+1) */
    if (k == 1u) ++val;                  /* +x term */
    if (r == 0u && k == 0u) ++val;       /* extra constant from x⁰ when r = 0 */
    if (k == r) ++val;                   /* xⁿ term */

    return val;
}
