/*
 * A239148  Expansion of triangle T(n,k) of p-adic valuations of A052129(n)
 *          (Somos' quadratic recurrence sequence).
 * https://oeis.org/A239148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   T(n,k) = v_p(A052129(n)) with p = (k+1)-th prime, k = 0..pi(n)-1
 *            (rows 0,1 are [0]); v_p(A052129(n)) = sum_{j=1..n}
 *            v_p(j)*2^(n-j), i.e. v(n) = 2*v(n-1) + v_p(n), v(0)=0. a(n) is
 *            the flattened triangle read by rows
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A239148_isp(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t r = 0;
    uint64_t pi = 0; /* number of primes <= r */
    for (;;) {
        if (r >= 2 && A239148_isp(r)) ++pi;
        uint64_t len = (r < 2) ? 1 : pi;
        if (len == 0) len = 1; /* not reachable for r>=2 */
        if (m < len) break;
        m -= len;
        ++r;
    }
    /* find (m+1)-th prime */
    uint64_t p = 1, cnt = 0;
    while (cnt <= m) {
        ++p;
        if (A239148_isp(p)) ++cnt;
    }
    seq_t v = 0;
    for (uint64_t k = 1; k <= r; ++k) {
        uint64_t t = k, e = 0;
        while (t % p == 0) { t /= p; ++e; }
        v = v * 2 + (seq_t)e;
    }
    return v;
}
