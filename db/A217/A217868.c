/*
 * A217868  a(n) is the sum of total number of nonnegative integer solutions
 *          to each of a^2 + b^2 = n, a^2 + 2*b^2 = n, a^2 + 3*b^2 = n and a^2
 *          + 7*b^2 = n. (Order matters for the equation a^2+b^2 = n).
 * https://oeis.org/A217868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over k∈{1,2,3,7} of the number of non‑negative
 *            integer * solution pairs (a,b) to a^2 + k*b^2 = n (ordered for k
 *            = 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t l = 0, r = n;
    while (l <= r) {
        idx_t m = l + ((r - l) >> 1);
        idx_t m2 = m * m;
        if (m2 == n) return m;
        if (m2 < n) l = m + 1;
        else r = m - 1;
    }
    return r; // floor sqrt
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    const idx_t ks[4] = {1, 2, 3, 7};
    for (int i = 0; i < 4; ++i) {
        idx_t k = ks[i];
        for (idx_t a = 0;; ++a) {
            idx_t asq = a * a;
            if (asq > n) break;
            idx_t r = n - asq;
            if (r % k != 0) continue;
            idx_t t = r / k;
            idx_t bsq = isqrt_u64(t);
            if (bsq * bsq == t) {
                sum += 1;
            }
        }
    }
    return sum;
}
