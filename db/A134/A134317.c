/*
 * A134317  Triangle, A128174 * A134309 as infinite lower triangular matrices.
 * https://oeis.org/A134317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) enumerates the triangle T(r,k) defined by T(r,1) = 1,
 *            T(r,k) = 2^(k-2) (k ≥ 2) if r + k is even, T(r,k) = 0 otherwise,
 *            with the entries listed in row‑major order (row 1 has length 1,
 *            row 2 has length 2, etc.)
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper floor_sqrt from the utils library. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; safety guard */
        return (seq_t)0;

    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2.
       Using the identity r = ⌊(√(8n−7)+1)/2⌋. */
    seq_t tmp = (seq_t)(((__int128)8 * n) - (seq_t)7);
    seq_t s   = floor_sqrt(tmp);
    idx_t r   = (idx_t)((s + 1) >> 1);

    /* Column index within the row: c = n − r(r−1)/2. */
    idx_t prev = (idx_t)r * (r - 1) / 2;
    idx_t c    = n - prev;

    /* Compute the triangle entry. */
    if ( (r + c) & 1 )
        return (seq_t)0;                          /* parity condition */
    if (c == 1)
        return (seq_t)1;                          /* first column */
    return (seq_t)1 << (c - 2);                  /* powers of two */
}
