/*
 * A127948  Triangle, A004736 * A127899.
 * https://oeis.org/A127948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = (1 - t) + 2*r, where * t = floor((sqrt(8*n + 1) - 1) / 2)
 *            (the largest t with t(t+1)/2 ≤ n), * r = n - t(t+1)/2. This is
 *            equivalent to the OEIS definition of A127948
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t types used by idx_t and seq_t */

seq_t a(idx_t n)
{
    /* Find t by binary search: largest t with t*(t+1)/2 <= n */
    idx_t lo = 0;
    idx_t hi = n;              /* hi is an upper bound for t */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)          /* mid is a candidate, keep it */
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t t = lo;

    /* Compute r = n - t(t+1)/2 */
    __int128 tri_t = (__int128)t * (t + 1) / 2;
    idx_t r = n - (idx_t)tri_t;

    /* Compute a(n) = (1 - t) + 2*r, using a wider type to avoid overflow */
    __int128 val = (__int128)1 - (__int128)t + (__int128)2 * (__int128)r;

    return (seq_t)val;
}
