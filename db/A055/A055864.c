/*
 * A055864  Coefficient triangle for certain polynomials.
 * https://oeis.org/A055864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth term (row‑major) of the triangle a(n,m) =
 *            n^(m-1) * (n+1)^(n-m) for 1 ≤ m ≤ n. The index n maps to the
 *            pair (row,col) by n = 1 + 2 + … + (row-1) + col, 1 ≤ col ≤ row
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Find the smallest r such that r*(r+1)/2 ≥ N.
   N is 1‑based. */
static inline idx_t _triangle_row(idx_t N)
{
    idx_t lo = 1, hi = N;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* use 128‑bit to avoid overflow for large N */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= (__int128)N)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Return the nth term of the sequence. */
extern seq_t a(idx_t n)
{
    /* Determine the row r (1‑based) containing the nth term */
    idx_t r = _triangle_row(n);

    /* Sum of 1 + … + (r-1) */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;

    /* Column index within the row (1‑based). */
    idx_t col = n - (idx_t)prev_sum;

    /* Compute n^(m-1) * (n+1)^(n-m) */
    seq_t part1 = ipow((seq_t)r, (seq_t)(col - 1));
    seq_t part2 = ipow((seq_t)(r + 1), (seq_t)(r - col));
    return part1 * part2;
}
