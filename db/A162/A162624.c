/*
 * A162624  Triangle read by rows in which row n lists n terms, starting with
 *          n^4 + n - 1, such that the difference between successive terms is
 *          equal to n^4 - 1 = A123865(n).
 * https://oeis.org/A162624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let r be the row number and k the position in that row
 *            (1‑based), so that n = r(r-1)/2 + k with 1 ≤ k ≤ r. Then a(n) =
 *            k * r^4 + r - k = (r^4 – 1) * k + r . This implements the
 *            triangle definition: first term of row r = r^4 + r – 1 and
 *            successive terms increase by r^4 – 1
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 3652, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r containing the linear index idx (1‑based).  */
static
idx_t
_find_row(idx_t idx)               /* idx ≥ 1 */
{
    idx_t lo = 1, hi = idx;        /* hi can safely be set to idx          */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* compute Tmid = mid*(mid+1)/2 using 128‑bit to avoid overflow */
        __int128 t = ( __int128 )mid * (mid + 1) / 2;
        if (t >= ( __int128 )idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;                     /* row number r                           */
}

seq_t
a(idx_t n)                        /* linear index n, 1‑based */
{
    /* locate the row r that contains index n */
    idx_t r = _find_row(n);

    /* compute k = position inside the row (1‑based) */
    __int128 prev_tri = ( __int128 )(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri;           /* 1 ≤ k ≤ r */

    /* compute the term: a(n) = k*r^4 + r - k */
    seq_t rn   = (seq_t)r;        /* r */
    seq_t r2   = rn * rn;         /* r^2 */
    seq_t r4   = r2 * r2;         /* r^4 */
    seq_t ks   = (seq_t)k;        /* k */

    return ks * r4 + rn - ks;
}
