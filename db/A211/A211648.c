/*
 * A211648  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2=3n.
 * https://oeis.org/A211648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y), 1 ≤ w,x,y ≤ n, such
 *            that w^2 + x^2 + y^2 = 3n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: floor of square root for 128‑bit unsigned values. */
static inline idx_t floor_sqrt_uint128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)8589934591ULL; /* 2^33-1 is > sqrt(5.5e19) */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t count = 0;

    /* sqrt3n = floor(sqrt(3*n));  w runs 1..sqrt3n */
    unsigned __int128 thr = (unsigned __int128)3 * n;
    idx_t sqrt3n = floor_sqrt_uint128(thr);

    for (idx_t w = 1; w <= sqrt3n; ++w) {
        unsigned __int128 w2 = (unsigned __int128)w * w;
        unsigned __int128 remaining_total = thr - w2;
        if (remaining_total < 2) continue;          /* need at least 1^2+1^2 */

        /* maximum x such that x^2 ≤ remaining_total-1 */
        unsigned __int128 rem_minus1 = remaining_total - 1;
        idx_t max_x = floor_sqrt_uint128(rem_minus1);
        if (max_x > n) max_x = n;

        for (idx_t x = 1; x <= max_x; ++x) {
            unsigned __int128 x2 = (unsigned __int128)x * x;
            unsigned __int128 y2 = remaining_total - x2;

            /* y is integer iff y2 is a perfect square */
            idx_t y = floor_sqrt_uint128(y2);
            if ((unsigned __int128)y * y == y2 && (idx_t)y <= n) {
                ++count;
            }
        }
    }
    return count;
}
