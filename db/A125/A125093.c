/*
 * A125093  Triangle T(n,k) = n*A054525(n,k) read by rows.
 * https://oeis.org/A125093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = n * μ(n/k) when k divides n, where k is the column index
 *            in row r that contains the n‑th element when the triangle
 *            T(r,k)=r*A054525(r,k) is read by rows. If k does not divide r
 *            the term is 0
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

static idx_t _row_for_idx(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while ( ( (__int128)hi * (hi + 1) ) / 2 < n )
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = _row_for_idx(n);
    idx_t first = r * (r - 1) / 2;    /* count of terms before row r */
    idx_t k = n - first;              /* 1‑based column in that row */

    if (r % k != 0)
        return 0;

    idx_t d = r / k;                  /* divisor n/k */
    seq_t mu = mobius(d);             /* Möbius of d, returns -1,0,1 */
    return (seq_t)r * mu;
}
