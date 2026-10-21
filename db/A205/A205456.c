/*
 * A205456  Symmetric matrix by antidiagonals: C(max(i,j),min(i,j)), i>=1,
 *          j>=1.
 * https://oeis.org/A205456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(max(i,j), min(i,j)) where (i,j) are the coordinates
 *            obtained by reading the symmetric matrix C(max(i,j),min(i,j)) by
 *            anti‑diagonals starting at (1,1)
 * Range:     u8 n <= 58, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t tri(idx_t k)
{
    /* triangular number k*(k-1)/2; use 128-bit to avoid overflow. */
    return (idx_t)(((unsigned __int128)k * (k - 1)) / 2);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset is 1; undefined for n=0 */

    /* Find the smallest s such that tri(s) >= n.
       s represents the current anti‑diagonal (i+j = s). */
    idx_t low = 2;
    idx_t high = n + 2;               /* guarantees high satisfies tri(high) >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of terms before the current diagonal. */
    idx_t prev = tri(s - 1);
    /* Position of the term within the current diagonal (1 .. s-1). */
    idx_t offset = n - prev;

    /* Coordinates on the diagonal: (i, j) = (offset, s - offset). */
    idx_t i = offset;
    idx_t j = s - offset;

    /* Binomial value at (i,j). */
    idx_t maxv = i > j ? i : j;
    idx_t minv = i < j ? i : j;
    return binomial(maxv, minv);
}
