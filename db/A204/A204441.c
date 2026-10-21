/*
 * A204441  Symmetric matrix: f(i,j)=floor[(i+j+2)/4]-floor[(i+j-1)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204441: 0‑1 symmetric matrix by antidiagonals
 *
 *  The table is defined by
 *      f(i,j) = floor[(i+j+2)/4] - floor[(i+j-1)/4],
 *  which equals 1 except when (i+j) ≡ 1 (mod 4) where it equals 0.
 *
 *  The sequence a(n) is obtained by writing the matrix in
 *  anti‑diagonal order starting with i=j=1:
 *      a(n) = 1  if the index k of the anti‑diagonal that contains the
 *                   n‑th cell satisfies k ≢ 1 (mod 4),
 *                0  otherwise.
 *  The anti‑diagonal index k is the unique integer with
 *      (k-1)(k-2)/2 < n ≤ k(k-1)/2 .
 *
 *  This implementation finds k by a binary search on the range
 *  [1, 2^32] using 128‑bit arithmetic to avoid overflow.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition (offset 1). */
    if (n == 0)
        return (seq_t)0;      /* safety for out of range values */

    /* Find the smallest k such that k(k-1)/2 ≥ n. */
    idx_t lo = 1, hi = 1;
    /* Expand high until it is large enough. */
    while ( (unsigned __int128)hi * (hi - 1u) / 2u < n )
        hi <<= 1u;               /* hi fits in 64 bits, max ~2^32 */

    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2u;
        if ( (unsigned __int128)mid * (mid - 1u) / 2u >= n )
            hi = mid;
        else
            lo = mid + 1u;
    }
    idx_t k = lo;               /* anti‑diagonal index */

    /* Determine if k ≡ 1 (mod 4). */
    unsigned int mod4 = k & 3u;
    seq_t result = (mod4 == 1u) ? (seq_t)0 : (seq_t)1;
    return result;
}
