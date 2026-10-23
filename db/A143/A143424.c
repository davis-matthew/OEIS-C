/*
 * A143424  Triangle read by rows, A054525 * A130123, 1<=k<=n.
 * https://oeis.org/A143424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The coefficient a(n) of A143424 is obtained by first translating the
 * linear index n into a pair (r, k) representing the row r and the k‑th
 * entry of that row (rows are enumerated starting at 1).  The triangle is
 * defined by
 *
 *   a(r, k) = μ(r / k) * 2^(k - 1)   if k | r
 *   a(r, k) = 0                      otherwise
 *
 * where μ is the Möbius function.  The sequence a(n) is the triangle
 * read by rows, i.e. a(n) = a(r, k) where r, k are obtained from n.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the smallest r with r*(r+1)/2 >= n  */
    idx_t low  = 1;
    idx_t high = 1;
    while ( ( (__int128)high * (high + 1) ) / 2 < n )
        high <<= 1;                         /* exponential search */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low  = mid + 1;
    }

    idx_t r = low;                            /* row number  (1‑based) */

    /* Convert to row‑column pair (k) */
    idx_t offset = (idx_t)( ( (__int128)(r - 1) * r ) / 2 ) + 1; /* first index of row r */
    idx_t k = n - offset + 1;

    /* If k does not divide r the entry is zero */
    if (r % k != 0) return 0;

    idx_t d = r / k;                          /* divisor for Möbius */
    seq_t  mu = mobius(d);                    /* μ(d)  (±1 or 0) */
    seq_t  powv = pow2(k - 1);                /* 2^(k-1) */

    return mu * powv;                         /* final term a(n) */
}
