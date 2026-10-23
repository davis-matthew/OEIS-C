/*
 * A199474  Leftmost column in the monotonic justified array of all positive
 *          generalized Fibonacci sequences (A160271).
 * https://oeis.org/A199474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (s(n)^2 - n) * [s(n)^2 - s(n) >= n] + (s(n)^2 - n + s(n))
 *            * [s(n)^2 - s(n) < n] where s(n) = ceil(sqrt(n))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor of sqrt(n) for n <= 2^64-1 */
static idx_t isqrt_floor(idx_t n) {
    idx_t low = 0, high = ((idx_t)1 << 32);
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* s = ceil(sqrt(n)) */
    idx_t s_floor = isqrt_floor(n);
    idx_t s = (s_floor * s_floor == n) ? s_floor : s_floor + 1;

    seq_t s2 = (seq_t)s * (seq_t)s;
    seq_t n_seq = (seq_t)n;
    seq_t candidate1 = s2 - n_seq;            /* s^2 - n   */
    seq_t candidate2 = candidate1 + (seq_t)s; /* s^2 - n + s */

    /* Test condition: (s^2 - s) >= n  <=>  s2 - s >= n_seq */
    if ((s2 - (seq_t)s) >= n_seq) {
        return candidate1;
    } else {
        return candidate2;
    }
}
