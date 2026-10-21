/*
 * A055136  Triangle: a(n,k) = A055135(n,k)/C(n,k).
 * https://oeis.org/A055136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by flattening the triangle a(n,k) =
 *            (-1)^(n+k)*(n-1)*(k-1)^(n-k-1) for 2 ≤ k ≤ n-1, with the
 *            following edge values: r = 0: a(0,0) = 1 r = 1: a(1,0) = 0 ,
 *            a(1,1) = 1 r ≥ 2: a(r,0) = -(r-1) a(r,1) = (r==2)?-1:0 a(r,r) =
 *            1 The linear index n is taken as the offset into the
 *            concatena...
 * Range:     i8 n <= 39, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   binary_search_row, pow_seqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binary_search_row.h"
#include "../utils/pow_seqt.h"

/* Helper: integer square root for unsigned __int128 */
static unsigned long long isqrt128(__uint128_t x)
{
    unsigned long long lo = 0, hi = ((unsigned long long)1 << 32);
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from linear index n */
    idx_t r = binary_search_row(n);
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* Special rows */
    if (r == 0) {
        return 1;
    }
    if (r == 1) {
        return (k == 0) ? 0 : 1;
    }

    /* r >= 2 */
    if (k == 0) {
        return -(seq_t)(r - 1);
    }
    if (k == 1) {
        return (r == 2) ? -1 : 0;
    }
    if (k == r) {
        return 1;
    }

    /* General case: 2 <= k <= r-1 */
    seq_t factor = (seq_t)(r - 1);              /* n-1 */
    seq_t base   = (seq_t)(k - 1);              /* k-1 */
    idx_t exp    = r - k - 1;                   /* n-k-1 */

    seq_t power = pow_seqt(base, exp);

    /* Sign: (-1)^(n+k) */
    seq_t sign = ((r + k) & 1) ? -1 : 1;

    return sign * factor * power;
}
