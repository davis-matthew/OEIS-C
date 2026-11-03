/*
 * A137225  Triangle T(k,q) of minimal q-Niven numbers: smallest number such
 *          that the sum of its digits in base q equals k, 2<=q<=k+1.
 * https://oeis.org/A137225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = T(k,q) where
 *   k = smallest integer with k*(k+1)/2 ≥ n
 *   offset = n - k*(k-1)/2
 *   q      = offset + 1
 *   T(k,q) = minimal positive integer whose sum of digits in base q = k
 *
 *   T(k,q) is computed by greedily filling the low-order digits with
 *   as much as possible (≤ q-1) to minimize the numeric value.
 */

#include <stdint.h>
#include <stdbool.h>   /* for bool, true, false */

/* helper: compute T(k,q) for given k>=1, q>=2 */
static inline seq_t
min_niven_value(idx_t k, idx_t q)
{
    seq_t value = 0;
    seq_t place = 1;          /* q^i for current digit position i */
    idx_t  rem   = k;
    idx_t  maxdig = q - 1;

    while (rem > 0) {
        idx_t digit = (rem <= maxdig) ? rem : maxdig;
        value += (seq_t)digit * place;
        rem   -= digit;
        place *= (seq_t)q;
    }
    return value;
}

seq_t
a(idx_t n)
{
    /* find k such that k*(k+1)/2 >= n */
    idx_t lo = 1, hi = n;   /* n ≥ 1 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1ULL) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t k = lo;

    /* compute offset in row k and corresponding base q */
    unsigned __int128 tri_prev = (unsigned __int128)(k - 1) * k / 2;
    idx_t offset = n - (idx_t)tri_prev;   /* 1 <= offset <= k */
    idx_t q = offset + 1;                 /* q ranges from 2 to k+1 */

    return min_niven_value(k, q);
}
