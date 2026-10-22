/*
 * A001004  Number of nonequivalent dissections of an (n+2)-gon by
 *          nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A001004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   with N = n+2, s = A001003 (little Schroeder) and D = A001850
 *            (central Delannoy): a(n) = ( s(N-2) + Sum_{d|N, d>1}
 *            phi(d)*D(N/d-1) + R(N) ) / (2N), where R(N) = N*s((N-1)/2) for N
 *            odd and R(N) = (N/2)*(s(N/2)+s(N/2-1)) for N even; a(0) = 1.
 *            s(0)=s(1)=1, (k+1)*s(k) = 3*(2k-1)*s(k-1) - (k-2)*s(k-2);...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   dz_add, dz_addw, dz_div, dz_mul, dz_phi, dz_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/dz_add.h"
#include "../utils/dz_addw.h"
#include "../utils/dz_div.h"
#include "../utils/dz_mul.h"
#include "../utils/dz_phi.h"
#include "../utils/dz_sub.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t N = n + 2;
    size_t L = (size_t)(3 * N / 32 + 8);
    uint32_t *buf = (uint32_t *)calloc(5 * L, sizeof(uint32_t));
    uint32_t *acc = buf, *t = buf + L;
    uint32_t *p2 = buf + 2 * L, *p1 = buf + 3 * L, *c = buf + 4 * L, *tmp;

    /* identity and reflections: little Schroeder numbers */
    for (idx_t k = 0; k <= N - 2; k++) {
        if (k < 2) {
            memset(c, 0, L * sizeof(uint32_t));
            c[0] = 1;
        } else {
            memcpy(c, p1, L * sizeof(uint32_t));
            dz_mul(c, L, (uint32_t)(3 * (2 * k - 1)));
            memcpy(t, p2, L * sizeof(uint32_t));
            dz_mul(t, L, (uint32_t)(k - 2));
            dz_sub(c, t, L);
            dz_div(c, L, (uint32_t)(k + 1));
        }
        uint32_t w = 0;
        if (k == N - 2) w += 1;
        if (N % 2) {
            if (k == (N - 1) / 2) w += (uint32_t)N;
        } else {
            if (k == N / 2 || k + 1 == N / 2) w += (uint32_t)(N / 2);
        }
        if (w) dz_addw(acc, c, t, L, w);
        tmp = p2; p2 = p1; p1 = c; c = tmp;
    }

    /* nontrivial rotations: central Delannoy numbers */
    memset(p2, 0, L * sizeof(uint32_t));
    memset(p1, 0, L * sizeof(uint32_t));
    for (idx_t k = 0; k + 1 <= N / 2; k++) {
        if (k < 2) {
            memset(c, 0, L * sizeof(uint32_t));
            c[0] = (k == 0) ? 1 : 3;
        } else {
            memcpy(c, p1, L * sizeof(uint32_t));
            dz_mul(c, L, (uint32_t)(3 * (2 * k - 1)));
            memcpy(t, p2, L * sizeof(uint32_t));
            dz_mul(t, L, (uint32_t)(k - 1));
            dz_sub(c, t, L);
            dz_div(c, L, (uint32_t)k);
        }
        idx_t q = k + 1;
        if (N % q == 0) dz_addw(acc, c, t, L, (uint32_t)dz_phi(N / q));
        tmp = p2; p2 = p1; p1 = c; c = tmp;
    }

    dz_div(acc, L, (uint32_t)(2 * N));

    seq_t r = 0;
    for (size_t i = L; i-- > 0;) {
        r = r * 65536 + (idx_t)(acc[i] >> 16);
        r = r * 65536 + (idx_t)(acc[i] & 0xFFFFu);
    }
    free(buf);
    return r;
}
