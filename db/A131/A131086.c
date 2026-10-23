/*
 * A131086  Triangle read by rows: T(n,k) = 2*binomial(n,k) - (-1)^(n-k) (0 <=
 *          k <= n).
 * https://oeis.org/A131086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2*binomial(n,k) - (-1)^(n-k) a(n) is the sequence read
 *            by rows of T(0,0), T(1,0),T(1,1),T(2,0),
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic integer overflow‑safe arithmetic for 64‑bit indices. */
#include <stdint.h>
#include "../utils/triangular.h"

/* Find the row r such that triangular(r) ≤ n < triangular(r+1). */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while (triangular(hi) <= n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Compute binomial coefficient C(n,k) multiplicatively.
   Uses seq_t for all arithmetic; works for arbitrary‑precision seq_t. */
static inline seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main OEIS A131086 sequence function. */
seq_t a(idx_t n)
{
    idx_t r   = find_row(n);                      /* row index    */
    idx_t pos = n - triangular(r);               /* column index */
    seq_t bin = binom_simple(r, pos);             /* C(r,k)       */
    seq_t val = bin + bin;                        /* 2*C(r,k)     */
    if (( (r - pos) & 1 ) == 0)                  /* exponent even */
        val -= (seq_t)1;                          /* subtract 1   */
    else
        val += (seq_t)1;                          /* add 1        */
    return val;
}
