/*
 * A072499  Product of divisors of n which are <= n^(1/2).
 * https://oeis.org/A072499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π{ d | n , d ≤ √n } d (product of divisors of n not
 *            exceeding its square‑root)
 * Range:     u8 n <= 59, u16 n <= 143, u32 n <= 503, u64 n <= 1679,
 *            u128 n <= 7559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1) + 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* for the OEIS offset a(1)=1, so n ≥ 1 */
    if (n == 0) return 0;          /* defensive, not used */
    idx_t lim = isqrt_u64(n);
    seq_t product = (seq_t)1;
    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i == 0) {
            product *= (seq_t)i;
        }
    }
    return product;
}
