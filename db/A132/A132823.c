/*
 * A132823  Triangle read by rows: T(n,k) = binomial(n,k) - 2 for 0 < k < n,
 *          with T(n,0) = T(n,n) = 1.
 * https://oeis.org/A132823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read by rows of triangle T(r,k) where T(r,0)=T(r,r)=1 and
 *            T(r,k)=binom(r,k)-2 for 0<k<r
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 94 terms through n = 1320 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for the row number. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high == (idx_t)(-1)) break;   /* prevent overflow if n is huge */
        high <<= 1;
    }

    /* Binary–search the smallest high with triangular > n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid;
    }

    idx_t r = low - 1;  /* row index */

    /* Starting index of row r. */
    unsigned __int128 off128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;

    idx_t k = n - offset;  /* column within the row */

    if (k == 0 || k == r)        /* edges of the triangle */
        return (seq_t)1;

    seq_t val = binomial(r, k);     /* interior value */
    return val - (seq_t)2;
}
