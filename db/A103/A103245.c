/*
 * A103245  Triangle read by rows: T(n,k) = binomial(2n+1,
 *          n-k)*Fibonacci(2k+1), 0 <= k <= n.
 * https://oeis.org/A103245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(r,k) =
 *            binomial(2r+1, r-k) * Fibonacci(2k+1) row by row
 * Range:     u8 n <= 14, u16 n <= 38, u32 n <= 125, u64 n <= 445,
 *            u128 n <= 1673, big any n
 * Validated: standard, pass, 62 terms through n = 8379 (2026-11-01)
 * Helpers:   binomial, fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/fibonacci.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute the triangle entry T(r,k). */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* binomial(2r+1, r-k) */
    seq_t bin = binomial(2 * r + 1, r - k);

    /* Fibonacci(2k+1) */
    seq_t f    = fibonacci(2 * k + 1);

    return bin * f;
}

/* Main expose: sequence a(n). */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th flattened entry. */
    idx_t r = row_of(n);

    /* Starting index of this row. */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t k = n - start;

    /* Return the triangle entry T(r,k). */
    return compute_T(r, k);
}
