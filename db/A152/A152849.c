/*
 * A152849  Triangle read by rows, A007318 rows repeated eleven times .
 * https://oeis.org/A152849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(r, j) where r is the row index of the repeated
 *            Pascal triangle row that contains position n, and j is the index
 *            within that row. Each row of A007318 is repeated exactly eleven
 *            times before moving to the next row
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;        /* current row index */
    idx_t cum = 0;      /* cumulative length of all previous rows */

    /* Find the row r such that cum + 11*(r+1) > n */
    for (;;)
    {
        idx_t block = 11 * (r + 1);  /* length contributed by row r */
        if (cum + block > n)
            break;
        cum += block;
        r += 1;
    }

    idx_t offset = n - cum;           /* position within row r block */
    idx_t j = offset % (r + 1);       /* position within a single row copy */
    return binomial(r, j);
}
