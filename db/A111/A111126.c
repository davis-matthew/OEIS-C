/*
 * A111126  Triangle read by rows: T(k,s) =
 *          binomial(k+s,2s+1)*(2k-1)*(2k+1)/(2s+3), k >= 1, 0 <= s <= k-1.
 * https://oeis.org/A111126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(k+s,2*s+1)*(2k-1)*(2k+1)/(2s+3), where k =
 *            row_of(n) (smallest k with k(k+1)/2 >= n), s = n - (k-1)k/2 - 1
 *            (0‑based column in that row)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static idx_t row_of(idx_t n)            /* smallest k with k(k+1)/2 >= n */
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 v = (__int128)mid * (mid + 1) / 2;
        if (v >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;        /* undefined for n < 1 */

    idx_t k = row_of(n);                              /* 1‑based row */
    __int128 pref = (__int128)(k - 1) * k / 2;        /* terms before row k */
    idx_t s = (idx_t)(n - pref - 1);                  /* 0‑based column */

    seq_t bin = binom(k + s, 2 * s + 1);               /* binomial part */
    seq_t res = bin;
    res *= (seq_t)(2 * k - 1);                        /* (2k-1) */
    res *= (seq_t)(2 * k + 1);                        /* (2k+1) */
    res /= (seq_t)(2 * s + 3);                        /* divide by (2s+3) */

    return res;
}
