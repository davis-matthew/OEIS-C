/*
 * A204156  Symmetric matrix based on f(i,j)=max(3i-j, 3j-i), by
 *          antidiagonals.
 * https://oeis.org/A204156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = max(3*i - j, 3*j - i) - 1, where (i,j) is the lattice
 *            point obtained from the n‑th element in the antidiagonal
 *            traversal of the infinite matrix M(i,j) = max(3i-j,3j-i)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safety for offset 1 */

    /* Find smallest k with k*(k+1)/2 >= n  (k is diagonal index). */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high == 0) {            /* wrap‑around guard */
            high = UINT64_MAX;
            break;
        }
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t k = low;                                   /* diagonal of n */
    idx_t tprev = ((unsigned __int128)(k - 1) * k) / 2;/* elements before this diag */
    idx_t pos   = n - tprev - 1;                     /* position inside diag (0‑based) */

    seq_t i = (seq_t)(pos + 1);  /* row index */
    seq_t j = (seq_t)(k - pos);  /* column index */

    /* Compute max(3*i-j, 3*j-i) without unsigned underflow. */
    seq_t maxij = i > j ? i : j;
    seq_t minij = i > j ? j : i;

    seq_t val = 3 * maxij - minij - 1;
    return val;
}
