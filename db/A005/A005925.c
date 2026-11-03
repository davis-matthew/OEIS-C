/*
 * A005925  Theta series of diamond.
 * https://oeis.org/A005925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (theta_2^3 + theta_3^3 + theta_4^3)/2, computed by
 *            counting (u,v,w) in Z^3 with u^2+v^2+w^2 = n, where either u,v,w
 *            are all even and u+v+w == 0 (mod 4), or u,v,w are all odd and
 *            u+v+w == 3 (mod 4) (the diamond lattice D3 u (D3 +
 *            (1/2,1/2,1/2)), scaled by 2)
 * Range:     u8 n <= 1255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9526 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long a005925_isqrt(long long x) {
    long long r, lo = 0, hi = 1;
    if (x < 0) return -1;
    while (hi * hi <= x) hi *= 2;
    while (lo < hi) {
        r = (lo + hi + 1) / 2;
        if (r * r <= x) lo = r; else hi = r - 1;
    }
    return lo;
}

static int a005925_ok(long long u, long long v, long long w) {
    long long pu = u & 1, pv = v & 1, pw = w & 1;
    long long s = ((u + v + w) % 4 + 4) % 4;
    if (pu != pv || pv != pw) return 0;
    if (pu == 0) return s == 0;
    return s == 3;
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long r = a005925_isqrt(N);
    seq_t cnt = 0;
    long long u, v;
    for (u = -r; u <= r; ++u) {
        long long rem1 = N - u * u;
        long long rv = a005925_isqrt(rem1);
        for (v = -rv; v <= rv; ++v) {
            long long rem2 = rem1 - v * v;
            long long w = a005925_isqrt(rem2);
            if (w < 0 || w * w != rem2) continue;
            if (a005925_ok(u, v, w)) cnt += 1;
            if (w != 0 && a005925_ok(u, v, -w)) cnt += 1;
        }
    }
    return cnt;
}
