/*
 * A131238  Triangle read by rows: T(n,k) = 2*binomial(n,k) -
 *          binomial(floor((n+k)/2), k) (0 <= k <= n).
 * https://oeis.org/A131238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * binomial(r,k) – binomial(floor((r+k)/2),k) where r =
 *            row number of flattened index n (T(r,0),…,T(r,r)) and k =
 *            position in that row (0 ≤ k ≤ r)
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 5150, big any n
 * Helpers:   _binom_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_binom_nk.h"

/* ----- helper: find the row number r for flattened index n ----- */
static idx_t _row_from_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* find an upper bound for r */
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* binary search in [low,high) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    idx_t r = _row_from_index(n);
    idx_t row_start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - row_start;                 /* 0 ≤ k ≤ r */

    seq_t bin1 = _binom_nk(r, k);
    idx_t f = (r + k) >> 1;                  /* floor((r+k)/2) */
    seq_t bin2 = _binom_nk(f, k);

    return (seq_t)2 * bin1 - bin2;           /* T(r,k) */
}
