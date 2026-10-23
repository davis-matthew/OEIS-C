/*
 * A155454  Triangle read by rows: t(n,k)= k! + (1-k!)*(n - k)!, 0<=k<=n.
 * https://oeis.org/A155454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = t(r,k) where r = floor((sqrt(8n+1)-1)/2) and k = n -
 *            r(r+1)/2; t(r,k) = k! + (1 - k!)*(r-k)! for 0 <= k <= r
 * Range:     i8 n <= 37, i16 n <= 56, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Determine the row r using a floating‑point approximation,
       then correct for any rounding error. */
    long double dn   = (long double) n;
    long double root = sqrtl((long double)8 * dn + 1);
    idx_t r = (idx_t)((root - 1) / 2);

    /* Compute the offset of the first element of row r. */
    __int128 prod   = (__int128) r * (r + 1);
    idx_t offset = (idx_t)(prod / 2);
    idx_t k = n - offset;

    /* If the approximation was off by one, adjust r and recompute. */
    if (k > r) {
        r++;
        prod   = (__int128) r * (r + 1);
        offset = (idx_t)(prod / 2);
        k = n - offset;
    }

    seq_t fk   = factorial(k);
    seq_t frk  = factorial(r - k);
    seq_t one  = (seq_t)1;
    seq_t result = fk + (one - fk) * frk;

    return result;
}
