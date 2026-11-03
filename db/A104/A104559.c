/*
 * A104559  Triangle, read by rows, of the number of left factors of peakless
 *          Motzkin paths of length n having k number of U's and D's (i.e.,
 *          number of paths from (0,0) to the line x=n, consisting of steps
 *          U=(1,1), H=(1,0), D=(1,1), that never go below the x-axis and a U
 *          step is never followed by a D step).
 * https://oeis.org/A104559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = row_of(n) k = n - r*(r+1)/2 and T(r, k)
 *            = C(r - floor(k/2), floor((k+1)/2)) * C(r - floor((k+1)/2),
 *            floor(k/2))
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A104559 */
#include <stdint.h>
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

seq_t a(idx_t n) {
    /* Determine which row the index n falls in */
    idx_t r = row_of(n);
    /* Position within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute floor(k/2) and floor((k+1)/2) */
    idx_t u = k / 2;          /* floor(k/2)  */
    idx_t v = (k + 1) / 2;    /* floor((k+1)/2) */

    /* Compute the binomial products as per the formula */
    seq_t t1 = binomial(r - u, v);
    seq_t t2 = binomial(r - v, u);

    return t1 * t2;
}
