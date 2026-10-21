/*
 * A203949  Symmetric matrix based on (1,1,0,1,1,0,1,1,0,...), by
 *          antidiagonals.
 * https://oeis.org/A203949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of positions where the periodic sequence s =
 *            (1,1,0,1,1,0,…) overlaps itself with offset determined by the
 *            (i,j) that would be at position n in a diagonal (antidiagonal)
 *            enumeration of the self‑fusion matrix M = T′*T
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>    /* for bool */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find minimal k with k*(k+1)/2 >= n.  k is the diagonal index. */
    idx_t k = 1;
    while (((__int128)k * (k + 1)) / 2 < n)
        ++k;

    /* Number of elements before this diagonal. */
    idx_t sum_prev = ((idx_t)(((__int128)(k - 1) * k) / 2));

    /* Zero‑based offset within diagonal k. */
    idx_t offset = n - sum_prev - 1;

    /* Coordinates (i,j) of the n‑th matrix entry. */
    idx_t i = offset + 1;
    idx_t j = k - offset;

    /* min(i,j) and the diagonal offset |i-j|. */
    idx_t m     = (i < j) ? i : j;
    idx_t delta = (i > j) ? (i - j) : (j - i);

    seq_t val = 0;

    /* For residues r = 1 and r = 2 (pattern values 1),
       count how many positions t (1 ≤ t ≤ m) satisfy
       both s[t] = 1 and s[t+delta] = 1. */
    for (idx_t r = 1; r <= 2; ++r) {
        if (((r + (delta % 3)) % 3) != 0) {       /* s[t+delta] must also be 1 */
            if (m >= r) {
                val += (seq_t)(((m - r) / 3) + 1);
            }
        }
    }

    return val;
}
