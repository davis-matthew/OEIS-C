/*
 * A152905  Irregular triangle read by rows, numbers in A007318 repeated .
 * https://oeis.org/A152905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r, k), where r and k are obtained from n by
 *            treating the array as rows of duplicated binomial coefficients:
 *            row r has 2·(r+1) elements and the first element of row r is
 *            item r·(r+1). The pair index within the row is k = (n – r·(r+1))
 *            / 2. (binomial is supplied by the utils library.)
 * Range:     u8 n <= 79, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"     /* provides seq_t binomial(idx_t n, idx_t k) */

/* Find the row r containing index n.  r satisfies
   r*(r+1) ≤ n < (r+1)*(r+2).  */
static idx_t row_from_n(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Increase high until it exceeds the desired row.  */
    while ((__int128)high * (high + 1) <= n) {
        high <<= 1;
    }

    /* Binary search on [low, high).  */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (((__int128)mid * (mid + 1)) <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row and position within that row.  */
    idx_t r      = row_from_n(n);
    idx_t offset = n - (idx_t)((__int128)r * (r + 1));

    /* Each binomial coefficient appears twice in a row.  */
    idx_t k = offset >> 1;   /* offset / 2 */

    return binomial(r, k);
}
