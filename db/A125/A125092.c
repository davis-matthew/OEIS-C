/*
 * A125092  Triangle read by rows: T(n,k) = (k+1)^2*binomial(n,k) (0 <= k <=
 *          n).
 * https://oeis.org/A125092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by rows from the triangle * T(r,k) = (k+1)^2 *
 *            binomial(r,k). The triangle has rows 0,1,2,…, * each row r
 *            containing r+1 elements (k = 0..r). The global index n * starts
 *            at 0
 * Range:     u8 n <= 23, u16 n <= 96, u32 n <= 391, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the total number of elements in rows
     * 0 .. r-1 is <= n < total elements in rows 0 .. r.
     * Row r contains r+1 entries. */
    idx_t r = 0;
    idx_t terms_before = 0;
    while ((terms_before + (r + 1)) <= n) {
        terms_before += r + 1;
        ++r;
    }

    /* Column index k within that row. */
    idx_t k = n - terms_before;

    /* Compute (k+1)^2 * binomial(r,k) using seq_t arithmetic. */
    seq_t bin = binomial(r, k);
    seq_t inc = (seq_t)(k + 1);
    return bin * inc * inc;
}
