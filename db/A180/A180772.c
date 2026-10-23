/*
 * A180772  Number of distinct solutions to the congruence x(1)*x(2) == 0 (mod
 *          n), with x() only in 1..n-1.
 * https://oeis.org/A180772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (a,b) with 1 <= a <= b <= n-1 *
 *            such that a*b ≡ 0 (mod n). Equivalent to * a(n) = Σ_{a=1}^{n-1}
 *            max(0, floor((n-1)/(n/gcd(a,n))) - ceil(a/(n/gcd(a,n))) + 1 )
 * Range:     u8 n <= 119, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sum = 0;

    for (idx_t a_val = 1; a_val < n; ++a_val) {
        idx_t g = gcd_idx(a_val, n);
        idx_t m = n / g;                     /* multiple step */
        idx_t max_b = (n - 1) / m;           /* floor((n-1)/m)   */
        idx_t min_b = (a_val + m - 1) / m;   /* ceil (a/m)      */

        if (max_b >= min_b) {
            idx_t cnt = max_b - min_b + 1;
            sum += (seq_t)cnt;
        }
    }

    return sum;
}
