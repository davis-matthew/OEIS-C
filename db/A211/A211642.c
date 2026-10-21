/*
 * A211642  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2<2n.
 * https://oeis.org/A211642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y)∈[1,n]^3 | w^2 + x^2 + y^2 < 2n}
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * mid <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t two_n = 2 * n;
    seq_t total = 0;

    /* Maximum w such that w^2 + 1^2 + 1^2 < 2n  →  w^2 < 2n - 3 */
    idx_t w_max = _isqrt_u64(two_n - 3);
    if (w_max > n) w_max = n;

    for (idx_t w = 1; w <= w_max; ++w) {
        idx_t w_sq   = w * w;
        idx_t rem_w  = two_n - w_sq;          /* remaining capacity for x^2 + y^2 */

        /* Need w^2 + 1^2 + 1^2 < 2n  →  rem_w > 2 */
        if (rem_w <= 2) continue;

        /* For a fixed w, maximum x satisfies x^2 < rem_w - 2  →  x^2 <= rem_w - 3 */
        idx_t x_max = _isqrt_u64(rem_w - 2);
        if (x_max > n) x_max = n;

        for (idx_t x = 1; x <= x_max; ++x) {
            idx_t x_sq = x * x;
            idx_t rem  = rem_w - x_sq;         /* remaining capacity for y^2 */

            /* Need y^2 < rem  →  rem > 1 */
            if (rem <= 1) continue;

            idx_t y_max = _isqrt_u64(rem - 1);
            if (y_max > n) y_max = n;

            total += (seq_t)y_max;
        }
    }

    return total;
}
