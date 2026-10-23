/*
 * A101491  Triangle T(n,k), read by rows: number of Knödel walks starting at
 *          0, ending at k, with n steps.
 * https://oeis.org/A101491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where row and col are obtained from n via n =
 *            row(row+1)/2 + col. For k=0: T(r,0)=∑_{i=0}^{r}
 *            (-1)^(r-i)*C(i,floor(i/2)). For k>0: T(r,k)=C(r,floor((r-k)/2)).
 *            The sequence a(n) is read by rows of this triangle
 * Range:     u8 n <= 35, u16 n <= 189, u32 n <= 566, u64 n <= 2345,
 *            u128 n <= 8777, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Locate the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (1) {
        idx_t next = (r + 1) * (r + 2) / 2;
        if (next > n)
            break;
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t col   = n - start;          /* column index in the triangle */

    if (col == 0) {
        /* Compute T(r,0) = Σ (-1)^(r-i) * C(i,floor(i/2)) without overflow. */
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;
        for (idx_t i = 0; i <= r; ++i) {
            idx_t k   = i / 2;
            seq_t ti  = binom(i, k);
            unsigned __int128 term = (unsigned __int128)ti;
            if (((r - i) & 1) == 0)
                pos += term;
            else
                neg += term;
        }
        unsigned __int128 res = pos - neg;
        return (seq_t)res;
    } else {
        /* For k>0, T(r,k)=C(r, floor((r-k)/2)). */
        idx_t k = (r - col) / 2;
        return binom(r, k);
    }
}
