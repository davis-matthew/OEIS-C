/*
 * A216209  Triangle read by rows: T(n,k) = n+k with 0 <= k <= 2*n.
 * https://oeis.org/A216209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor(sqrt(n)) - floor(sqrt(n))^2 + n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    idx_t s = 0;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t mid_sq = mid * mid;
        if (mid_sq <= n) {
            s = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    seq_t sn = (seq_t)n;
    seq_t ss = (seq_t)s;
    return sn + ss - ss * ss;
}
