/*
 * A132945  Triangle read by rows: T(n,m) = (m+1)^n*m^(n*(n-1)/2).
 * https://oeis.org/A132945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (m+1)^r * m^(r*(r-1)/2) where r is the unique row index *
 *            such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and m = n - r*(r+1)/2;
 *            * with T(r,0) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 35,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        ++r;
    --r;                       /* r is now the correct row index */

    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;       /* position within the row */

    if (m == 0)                /* row entries at m=0 are always 1 */
        return (seq_t)1;

    seq_t base1 = (seq_t)(m + 1);
    seq_t base2 = (seq_t)m;

    seq_t pow1 = seq_pow(base1, r);
    seq_t pow2 = seq_pow(base2, (r * (r - 1)) / 2);

    return pow1 * pow2;
}
