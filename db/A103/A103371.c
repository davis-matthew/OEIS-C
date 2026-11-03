/*
 * A103371  Number triangle T(n,k) = C(n,n-k)*C(n+1,n-k).
 * https://oeis.org/A103371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r, k) * C(r+1, k+1) where r = row_of(n) (the unique r
 *            with r(r+1)/2 ≤ n < (r+1)(r+2)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 179, u64 n <= 644,
 *            u128 n <= 2309, big any n
 * Validated: standard, pass, 76 terms through n = 6330 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helpers provided by the build environment */
#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);                           /* row index */
    idx_t k = n - r * (r + 1) / 2;                 /* column index */
    seq_t b1 = binomial(r, k);                        /* C(r, k)    */
    seq_t b2 = binomial(r + 1, k + 1);                /* C(r+1, k+1)*/
    return b1 * b2;                                /* T(r, k)    */
}
