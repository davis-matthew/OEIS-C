/*
 * A163233  Two-dimensional Binary Reflected Gray Code: a(i,j) = bits of
 *          binary expansion of A003188(i) interleaved with that of
 *          A003188(j).
 * https://oeis.org/A163233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = 2*interleave(A003188(i)) + interleave(A003188(j)) where
 *            (i,j) are the antidiagonal coordinates of n
 * Range:     u8 n <= 135, u16 n <= 2079, u32 n <= 2079, u64 n <= 2079,
 *            u128 n <= 2079, big any n
 * Helpers:   gray_from_index, interleave
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gray_from_index.h"
#include "../utils/interleave.h"

/* Find the antidiagonal indices (i,j) for the given enumeration index n.
   Uses a binary search for k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2.  */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }

    idx_t k = lo;
    unsigned __int128 start128 = (unsigned __int128)k * (k + 1) / 2;
    idx_t start = (idx_t)start128;

    idx_t offset = n - start;
    *i = offset;
    *j = k - offset;
}

/* Sequence function.  */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t gi = gray_from_index(i);
    seq_t gj = gray_from_index(j);

    seq_t ix = interleave(gi);
    seq_t iy = interleave(gj);

    /* Compute the final value using an unsigned 128‑bit intermediate
       to avoid any undefined shift on small seq_t types.  */
    unsigned __int128 res = ((unsigned __int128)ix << 1) + (unsigned __int128)iy;
    return (seq_t)res;
}
