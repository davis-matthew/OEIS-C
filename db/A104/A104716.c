/*
 * A104716  Triangle T(n,k) = (2k-3+4n)*(k-1-n)*(k-2-n)/6, 1<=k<=n.
 * https://oeis.org/A104716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ((2k-3+4r)*(k-1-r)*(k-2-r))/6, where r is the row
 *            satisfying r(r+1)/2 ≥ n and k = n - r(r-1)/2
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r (1‑based) containing the nth entry. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position k within that row. */
    idx_t k = n - (r * (r - 1)) / 2;

    /* Perform the calculation in signed 64‑bit arithmetic to avoid
       unsigned underflow, then cast the positive result back to seq_t. */
    int64_t i_r = (int64_t)r;
    int64_t i_k = (int64_t)k;

    int64_t t1 = 2 * i_k - 3 + 4 * i_r;
    int64_t t2 = i_k - 1 - i_r;
    int64_t t3 = i_k - 2 - i_r;

    int64_t numer = t1 * t2 * t3;      /* always positive */
    seq_t result = (seq_t)(numer / 6); /* exact division by 6 */

    return result;
}
