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
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n‑th) term of the array read by antidiagonals (summing n+m). */

#include <stdint.h>
#include <stddef.h>

/* helper: compute a(i,j) where i >= 0, j >= 0 */
static seq_t a_element(idx_t i, idx_t j)
{
    if (i == 0 || i == 1)
        return 1;

    seq_t prev2 = 1;   /* a(0,j) */
    seq_t prev1 = 1;   /* a(1,j) */
    for (idx_t k = 2; k <= i; ++k) {
        seq_t cur = (seq_t)j * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

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
