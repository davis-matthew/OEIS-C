/*
 * A218220  Array a(n,m) read by antidiagonals where a(0,m)=a(1,m)=1 and
 *          a(n,m) = m*a(n-1,m)-a(n-2,m) for n>=2.
 * https://oeis.org/A218220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n,m) = m*a(n-1,m) - a(n-2,m) for n>=2, and a(0,m)=a(1,m)=1
 * Range:     i8 n <= 49, i16 n <= 97, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   a_element
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n‑th) term of the array read by antidiagonals (summing n+m). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a_element.h"

seq_t a(idx_t n)
{
    /* find the diagonal s such that T(s) = s*(s+1)/2 <= n < T(s+1) */
    idx_t s = 0;
    /* use __int128 to avoid overflow for very large n */
    while (( (__int128)s * (__int128)(s + 1) ) / 2 <= n)
        ++s;
    --s;           /* now T(s) <= n */

    /* position within the diagonal */
    idx_t t = (s * (s + 1)) / 2;   /* T(s) */
    idx_t k = n - t;               /* 0 <= k <= s */

    /* coordinates (i,j) with increasing i */
    idx_t i = k;
    idx_t j = s - k;

    return a_element(i, j);
}
