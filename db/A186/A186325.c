/*
 * A186325  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the squares and octagonal
 *          numbers. Complement of A186324.
 * https://oeis.org/A186325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let N = n*(3*n-2) // the n-th octagonal number s = ⌊√N⌋ //
 *            number of squares ≤ N o = #{ j ≥ 1 | j*(3*j-2) < N } //
 *            octagonal numbers strictly < N Then a(n) = s + o + 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 1, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid) {  // mid*mid <= n ?
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Count of octagonal numbers < N  (N >= 1) */
static inline idx_t oct_less_than(idx_t N) {
    if (N <= 1) return 0;
    /* Upper bound for j: roughly sqrt(N/3) + 2  */
    idx_t high = isqrt(N / 3) + 2;
    idx_t low = 1, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t gmid = (__uint128_t)mid * (3 * mid - 2);
        if (gmid < (__uint128_t)N) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid <= 1) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* N = n*(3*n-2)  */
    idx_t N = n * (3 * n - 2);
    idx_t s_count = isqrt(N);
    idx_t oct_lt   = oct_less_than(N);
    seq_t rank = (seq_t)s_count + (seq_t)oct_lt + 1;
    return rank;
}
