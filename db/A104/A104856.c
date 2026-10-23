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
 * Helpers:   binom, row_of_index
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

#include "../utils/binom.h"
#include "../utils/row_of_index.h"

seq_t a(idx_t n)
{
    /* Determine row r and column k corresponding to the flat index n */
    idx_t r = row_of_index(n);
    idx_t k = n - (r * (r + 1)) / 2;   /* floor division is fine for unsigned */

    /* Compute the triangle entry T(r,k) */
    seq_t val = binom(r, k);                 /* binom(r,k)   */
    val *= binom(k, k / 2);                 /* binom(k,floor(k/2)) */
    val *= binom(r - k, (r - k) / 2);        /* binom(r-k,floor((r-k)/2)) */

    return val;
}
