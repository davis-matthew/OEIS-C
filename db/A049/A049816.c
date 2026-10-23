/*
 * A049816  Triangular array T read by rows: T(n,k) = number of nonzero
 *          remainders when Euclidean algorithm acts on n and k, for k=1..n,
 *          n>=1.
 * https://oeis.org/A049816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals the number of non‑zero remainders produced by the
 *            Euclidean algorithm when applied to the pair of indices (r, c)
 *            that correspond to the position n in the triangular array
 *            defined by T(n,k). Here r is the row number (starting at 1) and
 *            c is the column number (starting at 1). The mappin...
 * Range:     u8 n <= 9796, u16 n <= 9796, u32 n <= 9796, u64 n <= 9796,
 *            u128 n <= 9796, big any n
 * Helpers:   count_nonzero_remainders
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- implementation -----------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_nonzero_remainders.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 safe */

    /* find row r: smallest r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* column c */
    unsigned __int128 prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t c = (idx_t)(n - (idx_t)prev);

    return count_nonzero_remainders(r, c);
}
