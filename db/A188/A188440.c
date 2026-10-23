/*
 * A188440  Triangle T(n,k) read by rows: number of size-k antisymmetric
 *          subsets of {1,2,...,n}.
 * https://oeis.org/A188440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, easy, nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the row and k the column in the
 *            triangle T(n,k) = 2^k * C(floor(n/2),k) flattened row by row
 * Range:     u8 n <= 58, u16 n <= 162, u32 n <= 519, u64 n <= 1916,
 *            u128 n <= 2600, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r   = 0;              /* current row */
    idx_t cum = 0;              /* cumulative number of entries before row r */

    /* Find the row containing the n‑th term in the flattened triangle. */
    while (cum + ((r / 2) + 1) <= n)
    {
        cum += (r / 2) + 1;
        ++r;
    }

    idx_t k = n - cum;          /* column within the row */
    idx_t m = r / 2;            /* floor(r/2) */

    seq_t val = binom(m, k);    /* C(floor(r/2), k) */
    val *= pow2(k);             /* multiply by 2^k */

    return val;
}
