/*
 * A081422  Triangle read by rows in which row n consists of the first n+1
 *          n-gonal numbers.
 * https://oeis.org/A081422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, look
 * Formula:   a(n) = ((n - 2)k^2 - (n - 4)k)/2 where n is the row index, k =
 *            offset + 1 with offset = n - n*(n+1)/2 is the position inside
 *            the row
 * Range:     u8 n <= 52, u16 n <= 1428, u32 n <= 5355, u64 n <= 5355,
 *            u128 n <= 5355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* idx_t is defined by the build, but we assume it is an unsigned 64‑bit integer.
   seq_t is the value type (uint8_t … unsigned __int128 or arbitrary precision).   */
seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms up to row r-1
       is <= n < cumulative up to row r.  Since rows small we can loop.     */
    idx_t r = 0;
    for (;; ++r) {
        /* cumulative terms upto row r: (r+1)(r+2)/2.  Stop when it exceeds n. */
        if (((r + 1) * (r + 2)) / 2 > n)
            break;
    }

    /* Terms before this row: r(r+1)/2. */
    idx_t prev = (r * (r + 1)) / 2;
    /* Position inside the row (0‑based). */
    idx_t offset = n - prev;
    /* k is 1‑based index within the row. */
    idx_t k = offset + 1;

    /* Compute the n‑gonal number for this row and k.  Use signed 64‑bit
       integers for intermediate results (safe for the expected ranges).   */
    long long rr = (long long)r;
    long long kk = (long long)k;
    long long val = ((rr - 2) * kk * kk - (rr - 4) * kk) / 2;

    return (seq_t)val;
}
