/*
 * A212434  Triangle read by rows, related to Stern's diatomic series A002487,
 *          or fusc(n). See Comments for precise definition.
 * https://oeis.org/A212434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fusc(k) if (2k-1) <= r <= (2k+1) else a(n) = 0, where r
 *            is the row containing the n-th element in the triangle (i.e. the
 *            least r such that r(r+1)/2 >= n) and k = n - (r-1)r/2 (the
 *            column index, 1 ≤ k ≤ r)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: Stern's diatomic series (fusc) */
static seq_t fusc(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((n & 1) == 0) {
        return fusc(n >> 1);
    } else {
        return fusc((n - 1) >> 1) + fusc((n + 1) >> 1);
    }
}

/* Helper: triangular number r*(r+1)/2 using 128‑bit to avoid overflow */
static inline __int128 triangular(idx_t r)
{
    return (__int128)r * (r + 1) / 2;
}

/* Given n >= 1, return the value a(n) defined by the triangle */
seq_t a(idx_t n)
{
    /* Find row r such that triangular(r-1) < n <= triangular(r) */
    idx_t lo = 1, hi = 2;
    while (triangular(hi) < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index within the row */
    idx_t k = n - (idx_t)triangular(r - 1);

    /* Check whether cell (r, k) contains fusc(k) according to the
       triangular shape described in the OEIS comment. */
    if ((__int128)r >= ((__int128)k << 1) - 1 &&
        (__int128)r <= ((__int128)k << 1) + 1) {
        return fusc(k);
    } else {
        return 0;
    }
}
