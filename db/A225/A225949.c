/*
 * A225949  Triangle for sum of the two legs (catheti) of primitive
 *          Pythagorean triangles.
 * https://oeis.org/A225949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) computed from triangle a(n,m) = (n+m)^2 - 2*m^2 for
 *            primitive Pythagorean triples; zero otherwise
 * Range:     u8 n <= 62, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(2) is the first term, index offset is n-2 */
    if (n < 2) return 0;

    idx_t offset = n - 2;          /* 0‑based position in row‑major order */

    /* Find row r (n in the triangle) such that offset < r*(r-1)/2. */
    idx_t r = 2;
    idx_t cumulative = 0;          /* number of elements before row r */

    while (offset >= cumulative + (r - 1)) {
        cumulative += (r - 1);
        r++;
    }

    /* Column index m (1 … r-1) */
    idx_t m = offset - cumulative + 1;

    /* Primitive conditions */
    if (((r + m) & 1) == 0) return 0;            /* same parity → not primitive */
    if (gcd((seq_t)r, (seq_t)m) != (seq_t)1) return 0;

    /* Compute (r+m)^2 - 2*m^2 using 128‑bit arithmetic to avoid overflow. */
    __int128 sum  = (__int128)r + (__int128)m;
    __int128 term = sum * sum - (__int128)2 * (__int128)m * (__int128)m;

    return (seq_t)term;
}
