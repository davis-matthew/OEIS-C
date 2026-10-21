/*
 * A090402  Symmetric triangle, read by rows, where the terms in each row,
 *          after being divided by their corresponding binomial coefficients,
 *          have an integer sum.
 * https://oeis.org/A090402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T( floor((sqrt(8*n+1)-1)/2), n -
 *            floor((sqrt(8*n+1)-1)/2)*(floor((sqrt(8*n+1)-1)/2)+1)/2 )
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T is defined by the symmetric triangle in OEIS A090402 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while (1) {
        __int128 tri = (__int128)(r + 1) * (r + 2) / 2;
        if (tri > n) break;
        r++;
    }

    /* Compute offset k within the row: index n = r(r+1)/2 + k  */
    __int128 pref = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)pref;            /* 0 <= k <= r */

    /* Handle the trivial first row */
    if (r == 0) {                         /* T(0,0) = 1 */
        return 1;
    }

    /* Compute the triangle entry T(r,k) by symmetry */
    idx_t m = r / 2;                      /* floor(r/2) */
    idx_t t = (k <= r - k) ? k : r - k;   /* min(k, r-k) */

    /* T(r,0) = T(r,r) = 1 */
    if (t == 0) {
        return 1;
    }

    if ((r & 1ULL) == 0) {                /* even row: r = 2*m */
        if (t == m) {                     /* middle element */
            return (seq_t)m + 1;          /* T(2m,m) = m+1 */
        } else {                           /* 0 < t < m */
            return (seq_t)m - t;          /* T(2m,t) = m-t */
        }
    } else {                               /* odd row: r = 2*m+1 */
        if (t == m) {                     /* two middle elements */
            return (seq_t)m + 2;          /* T(2m+1,m) = T(2m+1,m+1) = m+2 */
        } else {                           /* 0 < t < m */
            /* T(2m+1,t) = 2m+1 - 2*t = r - 2*t */
            return (seq_t)(r - 2 * t);
        }
    }
}
