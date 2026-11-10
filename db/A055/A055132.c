/*
 * A055132  Moebius function (A008683) applied to each term in the triangle
 *          A055096.
 * https://oeis.org/A055132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = mu(A055096(n+1)), where A055096 lists k^2 + r^2 for rows
 *            r >= 2, 1 <= k <= r-1, and mu is the Moebius function (A008683)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A055132_mu(uint64_t m)
{
    if (m == 1) return 1;
    int cnt = 0;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            ++cnt;
            m /= p;
            if (m % p == 0) return 0;
        }
    }
    if (m > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n - 1;  /* zero-based position in A055096 */
    /* largest t with t*(t-1)/2 <= idx; t = r-1 */
    uint64_t lo = 1, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 v = (unsigned __int128)mid * (mid - 1) / 2;
        if (v <= idx) lo = mid; else hi = mid - 1;
    }
    uint64_t t = lo;
    uint64_t r = t + 1;
    uint64_t k = idx - t * (t - 1) / 2 + 1;
    uint64_t val = k * k + r * r;
    return (seq_t)A055132_mu(val);
}
