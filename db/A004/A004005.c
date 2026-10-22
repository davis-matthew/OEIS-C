/*
 * A004005  Coefficients of elliptic function sn.
 * https://oeis.org/A004005
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (5^(2*n+1) - (8*n-4)*3^(2*n+1) + 32*n^2 - 32*n - 17)/256.
 *            Each big term is kept as 256*q + r (r < 256) so that no term
 *            overflows before the division by 256
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   pow_split
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_split.h"

seq_t a(idx_t n) {
    idx_t e = 2 * n + 1;
    seq_t qp, q3, qq, qt;
    uint64_t rp, r3, rq, rt, t, c, T;
    long long s;
    seq_t res;

    pow_split(5, e, &qp, &rp);
    pow_split(3, e, &q3, &r3);

    c = (uint64_t)(8 * n - 4);
    t = c * r3;
    qq = q3 * (idx_t)c + (seq_t)(t >> 8);
    rq = t & 255;

    T = 32 * (uint64_t)n * (uint64_t)n - 32 * (uint64_t)n - 17;
    qt = (seq_t)(T >> 8);
    rt = T & 255;

    s = (long long)rp + (long long)rt - (long long)rq;

    res = qp + qt;
    if (s > 0) res += (seq_t)(s / 256);
    res -= qq;
    if (s < 0) res -= (seq_t)((-s) / 256);
    return res;
}
