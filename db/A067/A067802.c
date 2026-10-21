/*
 * A067802  Triangle read by rows: T(n, k) = binomial(2*n+1,
 *          n-k)^2*(2*k+1)/(2*n+1).
 * https://oeis.org/A067802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where n = r*(r+1)/2 + k (0‑based rows and cols).
 *            T(r,k) = binomial(2*r+1, r-k)^2 * (2*k+1) / (2*r+1)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial function is provided by the build in "../utils/binomial.h". */
#include <stdint.h>
#include "../utils/binomial.h"

/* Determine 0‑based row r and column k for a linear index n.  */
static inline void row_col_from_index(idx_t n, idx_t *r, idx_t *k)
{
    idx_t low = 0;
    idx_t high = n;                 /* r cannot exceed n                */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }

    *r = low;
    *k = n - (unsigned __int128)low * (low + 1) / 2;
}

/* Compute the OEIS a(n) value for arbitrary precision or fixed‑width builds. */
seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);

    /* Compute the binomial coefficient C(2r+1, r-k). */
    seq_t bin = binomial((idx_t)(2 * r + 1), (idx_t)(r - k));

    /* T(r,k) = bin^2 * (2k+1) / (2r+1).  All operations in seq_t. */
    seq_t result = bin * bin;                         /* bin^2        */
    result *= (seq_t)(2 * k + 1);                     /* * (2k+1)     */
    result /= (seq_t)(2 * r + 1);                     /* / (2r+1)     */

    return result;
}
