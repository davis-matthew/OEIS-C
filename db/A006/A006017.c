/*
 * A006017  Nim product 2^n * 2^n.
 * https://oeis.org/A006017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nim-square of 2^n. If n = 2^t + r with 0 <= r < 2^t, then
 *            a(n) = a(r)*2^(2^t) XOR (a(r) (x) 2^(2^t - 1)), where (x) is the
 *            nim product in the field of size 2^(2^t); a(0) = 1. Multiplying
 *            by 2^(2^t - 1) uses H_t(x1*G + x0) = H_{t-1}(x0^x1)*G ^
 *            H_{t-1}(H_{t-1}(x1)), G = 2^(2^(t-1)), H_0(x) = x
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 248 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* x (x) 2^(2^t - 1) for x < 2^(2^t), t <= 6 */
static uint64_t h64(uint64_t x, int t) {
    int hb;
    uint64_t m, x1, x0;
    if (t == 0) return x;
    hb = 1 << (t - 1);
    m = (((uint64_t)1) << hb) - 1;
    x1 = x >> hb;
    x0 = x & m;
    return (h64(x0 ^ x1, t - 1) << hb) | h64(h64(x1, t - 1), t - 1);
}

/* same for t >= 6 on arrays of 2^(t-6) words; out must not alias x */
static void hbig(const uint64_t *x, uint64_t *out, int t) {
    size_t w, hw, i;
    uint64_t *tmp;
    if (t == 6) { out[0] = h64(x[0], 6); return; }
    w = (size_t)1 << (t - 6);
    hw = w / 2;
    tmp = (uint64_t *)malloc(hw * sizeof(uint64_t));
    for (i = 0; i < hw; i++) tmp[i] = x[i] ^ x[hw + i];
    hbig(tmp, out + hw, t - 1);
    hbig(x + hw, tmp, t - 1);
    hbig(tmp, out, t - 1);
    free(tmp);
}

seq_t a(idx_t n) {
    int T = 0, t, k;
    size_t nw, i, lw;
    uint64_t *v, *nv, *sw;
    seq_t r = 0;
    bool_dummy:;
    if (n == 0) return (seq_t)1;
    while ((n >> (T + 1)) != 0) T++;
    nw = (T + 1 <= 6) ? 1 : ((size_t)1 << (T + 1 - 6));
    v = (uint64_t *)calloc(nw, sizeof(uint64_t));
    nv = (uint64_t *)calloc(nw, sizeof(uint64_t));
    v[0] = 1;
    for (t = 0; t <= T; t++) {
        if (!((n >> t) & 1)) continue;
        if (t < 6) {
            v[0] = (v[0] << (1 << t)) ^ h64(v[0], t);
        } else {
            lw = (size_t)1 << (t - 6);
            hbig(v, nv, t);
            memcpy(nv + lw, v, lw * sizeof(uint64_t));
            sw = v; v = nv; nv = sw;
        }
    }
    for (i = nw; i-- > 0;) {
        for (k = 3; k >= 0; k--) {
            r = (seq_t)(r << 16);
            r = r | (seq_t)((v[i] >> (16 * k)) & 0xFFFFu);
        }
    }
    free(v);
    free(nv);
    return r;
}
