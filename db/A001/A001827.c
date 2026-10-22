/*
 * A001827  Related to graded partially ordered sets.
 * https://oeis.org/A001827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{q+r<=n} n!/(q! r! (n-q-r)!) * 2^(q*r) * (2^q +
 *            2^r)^(n-q-r) (from a(n) = Sum_{p+q+r+s=n} n!/(p!q!r!s!)
 *            2^(pq+qr+rs), summing over p+s by the binomial theorem)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   a001827_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001827_pw.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t cnq = 1;      /* C(n, q) */
    seq_t p2q = 1;      /* 2^q */
    for (idx_t q = 0; q <= n; q++) {
        idx_t m = n - q;
        seq_t cmr = 1;      /* C(m, r) */
        seq_t p2r = 1;      /* 2^r */
        seq_t p2qr = 1;     /* 2^(q*r) */
        for (idx_t r = 0; r <= m; r++) {
            seq_t t = cnq * cmr;
            t *= p2qr;
            t *= a001827_pw(p2q + p2r, m - r);
            sum += t;
            if (r < m) {
                cmr = cmr * (seq_t)(m - r) / (seq_t)(r + 1);
                p2r *= 2;
                p2qr *= p2q;
            }
        }
        if (q < n) {
            cnq = cnq * (seq_t)(n - q) / (seq_t)(q + 1);
            p2q *= 2;
        }
    }
    return sum;
}
