/*
 * A230417  Lower triangular region of A230415, a triangular table read by
 *          rows: T(n, k) tells in how many digit positions the factorial base
 *          representations (A007623) of n and k differ, where (n, k) = (0,0),
 *          (1,0), (1,1), (2,0), (2,1), (2,2), ..., n >= 0 and (0 <= k <= n).
 * https://oeis.org/A230417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = number of digit positions where the factorial base
 *            (A007623) representations of the row index r = ⌊(√(8n+1)−1)/2⌋
 *            and the column index c = n − r(r+1)/2 differ. Thus a(n) = T(r,c)
 *            with T(i,j) = #k such that the k‑th factorial digits of i and j
 *            are different
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   fact_digit_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fact_digit_diff.h"

static inline idx_t triangular_row(idx_t n)
{
    /* Find largest r such that r*(r+1)/2 <= n */
    idx_t r = 0;
    while (1) {
        unsigned __int128 value = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (value <= n) {
            r++;
        } else {
            break;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t row = triangular_row(n);
    idx_t col = n - (row * (row + 1) / 2);
    return fact_digit_diff(row, col);
}
