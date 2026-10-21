/*
 * A091913  Triangle read by rows: a(n,k) = C(n,k)*(2^(n-k) - 1) for k<n,
 *          a(n,k) = 0 for k >= n, where k=0..max(n-1,0).
 * https://oeis.org/A091913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = C(r,k)*(2^(r-k)-1) where (r,k) are the unique row r and
 *            column k such that the flattened index n lies in the triangle
 *            read by rows
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the very first element directly. */
    if (n == 0)
        return 0;

    /* Find the row r that contains the index n.
       Row lengths: 1,1,2,3,4,...  (len(0)=len(1)=1, len(r)=r for r>=2). */
    idx_t row = 0;
    unsigned __int128 cumulative = 0;
    for (;;)
    {
        idx_t len = (row <= 1) ? 1 : row;          /* length of current row   */
        if (cumulative + len > n)
            break;
        cumulative += len;
        ++row;
    }

    /* Column index within the row. */
    idx_t k = n - (idx_t)cumulative;

    /* Compute C(row,k) * (2^(row-k) - 1). */
    seq_t bin  = binomial(row, k);
    seq_t pw   = pow2(row - k);        /* 2^(row-k)                */
    seq_t diff = pw - 1;               /* 2^(row-k) - 1            */

    return bin * diff;
}
