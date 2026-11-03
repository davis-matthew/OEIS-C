/*
 * A104856  Triangle read by rows: T(n,k) =
 *          binomial(n,k)*binomial(k,floor(k/2))*binomial(n-k,floor((n-k)/2))
 *          (0<=k<=n).
 * https://oeis.org/A104856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) is the nth element of the triangle T(r,k) read row by row
 *           where
 *
 *           T(r,k) = binom(r,k) *
 *                    binom(k, floor(k/2)) *
 *                    binom(r-k, floor((r-k)/2))
 *
 *           r   = row_of_index(n)          // row number (0‑based)
 *           k   = n - r*(r+1)/2            // index within the row (0‑based)
 */

#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k corresponding to the flat index n */
    idx_t r = row_of_index(n);
    idx_t k = n - (r * (r + 1)) / 2;   /* floor division is fine for unsigned */

    /* Compute the triangle entry T(r,k) */
    seq_t val = binomial(r, k);                 /* binom(r,k)   */
    val *= binomial(k, k / 2);                 /* binom(k,floor(k/2)) */
    val *= binomial(r - k, (r - k) / 2);        /* binom(r-k,floor((r-k)/2)) */

    return val;
}
