/*
 * A178822  Triangle read by rows: T(n,k) = C(n+5,5) * C(n,k), 0 <= k <= n.
 * https://oeis.org/A178822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r+5,5) * C(r,k) where r = smallest row index with
 *            (r+1)(r+2)/2 > n k = n - r(r+1)/2 <= r (the triangle read by
 *            rows of T(n,k)=C(n+5,5)*C(n,k))
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 217, u64 n <= 1102,
 *            u128 n <= 1274, big any n
 * Validated: standard, pass, 61 terms through n = 1254 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute the n-th term of A178822. */
seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative count up to that row
       exceeds n.  The cumulative count of all rows up to r inclusive is
       (r+1)(r+2)/2. */
    idx_t r = 0;
    while ( ( (__uint128_t)(r + 1) * (r + 2) ) / 2 <= n )
        ++r;

    /* Column index k in this row. */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Compute the two binomial coefficients and multiply them. */
    seq_t top = binomial(r + 5, 5);   /* C(r+5,5) */
    seq_t bot = binomial(r, k);       /* C(r,k)   */

    return seq_mul(top, bot);
}
