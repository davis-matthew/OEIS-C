/*
 * A007251  McKay-Thompson series of class 5A for the Monster group.
 * https://oeis.org/A007251
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (eta(q)/eta(q^5))^6 + 6 + 125*(eta(q^5)/eta(q))^6,
 *            a(-1) = 1, a(0) = 0. With P3 = (eta(q^5)/eta(q))^3 * q^(1/2) and
 *            R = 1/P3 (Jacobi: prod(1-q^k)^3 = Sum (-1)^m (2m+1)
 *            q^(m(m+1)/2)), a(n) = [q^(n+1)] R^2 + 125 * [q^(n-1)] P3^2 for n
 *            >= 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 74, u128 n <= 277,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n == -1) return 1;
    if (n == 0) return 0;

    idx_t M = n + 1;
    idx_t m, t, T, i;

    /* h = 1/prod(1-q^k)^3, positive coefficients */
    seq_t *h = alloc_zero(M + 1);
    h[0] = 1;
    for (t = 1; t <= M; t++) {
        seq_t p = 0, q = 0;
        for (m = 1; (T = m * (m + 1) / 2) <= t; m++) {
            if (m & 1) p += h[t - T] * (seq_t)(2 * m + 1);
            else       q += h[t - T] * (seq_t)(2 * m + 1);
        }
        h[t] = p - q;
    }

    /* g = prod(1-q^(5k))^3 / prod(1-q^k)^3, positive coefficients */
    seq_t *g = alloc_zero(n);
    for (t = 0; t < n; t++) {
        seq_t p = 0, q = 0;
        for (m = 0; (T = 5 * (m * (m + 1) / 2)) <= t; m++) {
            if (m & 1) q += h[t - T] * (seq_t)(2 * m + 1);
            else       p += h[t - T] * (seq_t)(2 * m + 1);
        }
        g[t] = p - q;
    }

    /* r = prod(1-q^k)^3 / prod(1-q^(5k))^3 = rp - rn (signed coefficients) */
    seq_t *rp = alloc_zero(M + 1);
    seq_t *rn = alloc_zero(M + 1);
    for (t = 0; t <= M; t++) {
        seq_t p = 0, q = 0;
        for (m = 0; (T = m * (m + 1) / 2) <= t; m++) {
            if ((t - T) % 5 != 0) continue;
            if (m & 1) q += h[(t - T) / 5] * (seq_t)(2 * m + 1);
            else       p += h[(t - T) / 5] * (seq_t)(2 * m + 1);
        }
        if (p >= q) { rp[t] = p - q; rn[t] = 0; }
        else        { rn[t] = q - p; rp[t] = 0; }
    }

    seq_t Y = 0;
    for (i = 0; i < n; i++) Y += g[i] * g[n - 1 - i];

    seq_t same = 0, diff = 0;
    for (i = 0; i <= M; i++) {
        same += rp[i] * rp[M - i];
        same += rn[i] * rn[M - i];
        diff += rp[i] * rn[M - i];
        diff += rn[i] * rp[M - i];
    }

    seq_t result = same + Y * 125;
    result -= diff;

    release(h, M + 1);
    release(g, n);
    release(rp, M + 1);
    release(rn, M + 1);
    return result;
}
