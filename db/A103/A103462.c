/*
 * A103462  A triangle with palindromic cubes, read by rows.
 * https://oeis.org/A103462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, k) where T(n,k) = k^(n-k)+1-0^(n-k) for k≤n, else 0.
 *            * r = row_of_index(n) such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 * k
 *            = n - r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   compute_row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_row_of.h"

seq_t a(idx_t n)
{
    idx_t r = compute_row_of(n);
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;          /* start index of row r */
    idx_t k = n - start;                    /* column index, 0 ≤ k ≤ r */

    if (k == r)                /* diagonal entry */
        return (seq_t)1;

    seq_t val = (seq_t)1;      /* start with 1 for +1 term */
    idx_t exp = r - k;
    for (idx_t i = 0; i < exp; ++i)
        val *= (seq_t)k;      /* multiply by k^(r-k) */

    val += (seq_t)1;           /* add the +1 term (0^(r-k) is 0 for r≠k) */
    return val;
}
