/*
 * A131437  (A000012 * A131436) + (A131436 * A000012) - A000012.
 * https://oeis.org/A131437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^r + 2^(j+1) - 3 * where r is the unique integer such
 *            that r(r+1)/2 >= n * and j = n - r(r-1)/2 - 1. This is the
 *            row‐major ordering * of the triangle whose first element of row
 *            r is 2^r-1 and * successive differences are successive powers of
 *            two
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 0) return 0;               /* protection for accidental call */

    /* ---- Find r: the row containing the n-th element ---- */
    idx_t low  = 0;
    idx_t high = n;                      /* r <= n for any n >= 1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* 1‑based row index */

    /* ---- Compute column index j ---- */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)prev_tri - 1;    /* 0‑based column */

    /* ---- Compute the value: 2^r + 2^(j+1) - 3 ---- */
    seq_t val = pow2(r);
    val += pow2(j + 1);
    val -= 3;

    return val;
}
