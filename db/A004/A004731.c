/*
 * A004731  a(0) = 1; thereafter a(n) = denominator of (n-2)!! / (n-1)!!.
 * https://oeis.org/A004731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(0) = 1; a(2m+1) = 2^(2m - popcount(m)); a(2m) = odd part of
 *            m*binomial(2m,m), computed via r(1) = 1, r(k+1) =
 *            r(k)*(2k+1)/oddpart(k)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   a004731_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004731_gcd_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t m = n / 2;
    if (n % 2 == 1) {
        idx_t pc = 0, t = m;
        while (t) { pc += t & 1; t >>= 1; }
        idx_t e = 2 * m - pc;
        seq_t r = 1;
        r <<= e;
        return r;
    }
    /* n = 2m, m >= 1 */
    seq_t r = 1;
    idx_t k;
    for (k = 1; k < m; k++) {
        uint64_t ok = k;
        while ((ok & 1) == 0) ok >>= 1;
        uint64_t g = a004731_gcd_u64((uint64_t)(r % (seq_t)ok), ok);
        r /= (seq_t)g;
        uint64_t d = ok / g;
        uint64_t f = (2 * (uint64_t)k + 1) / d;
        r *= (seq_t)f;
    }
    return r;
}
