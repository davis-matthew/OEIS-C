/*
 * A080851  Square array of pyramidal numbers, read by antidiagonals.
 * https://oeis.org/A080851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the array * T(r,c) = (c+1)(c+2)/2 +
 *            r * c(c+1)(c+2)/6 * along antidiagonals. * For a given index n
 *            (0‑based), find the diagonal d such that * (d+1)(d+2)/2 > n and
 *            d(d+1)/2 ≤ n. * Let offset = n - d(d+1)/2. * Then r = d -
 *            offset, c = offset. * The value a(n) is computed...
 * Range:     u8 n <= 61, u16 n <= 976, u32 n <= 7823, u64 n <= 7823,
 *            u128 n <= 7823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* locate the diagonal d */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= (unsigned __int128)n)
        ++d;

    /* offset within the diagonal */
    idx_t tprev = d * (d + 1) / 2;     /* t(d-1) */
    idx_t offset = n - tprev;          /* 0 ≤ offset ≤ d */

    /* coordinates in the array */
    idx_t r = d - offset;   /* row */
    idx_t c = offset;       /* column */

    /* compute the value using 64‑bit intermediates to avoid
     * overflow of small types */
    uint64_t k   = (uint64_t)c;
    uint64_t base64   = (k + 1) * (k + 2) / 2;              /* (c+1)(c+2)/2 */
    uint64_t mult64   = (k + 2) * (k + 1) * k / 6;           /* c(c+1)(c+2)/6 */
    uint64_t total    = base64 + (uint64_t)r * mult64;

    return (seq_t)total;
}
