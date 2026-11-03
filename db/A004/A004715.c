/*
 * A004715  Positions of ones in binary expansion of arctan(1/2)/Pi.
 * https://oeis.org/A004715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the n-th 1 bit after the binary point of
 *            arctan(1/2)/Pi. Computed in fixed point: arctan(1/m) =
 *            Sum_{k>=0} (-1)^k/((2k+1) m^(2k+1)), Pi = 4*(arctan(1/2) +
 *            arctan(1/3)), then binary long division with guard bits
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 964 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a4715_add(uint32_t *a, const uint32_t *b, int L)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void a4715_divsmall(uint32_t *x, int L, uint32_t d)
{
    uint64_t r = 0;
    int i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int a4715_iszero(const uint32_t *x, int L)
{
    int i;
    for (i = 0; i < L; i++) if (x[i]) return 0;
    return 1;
}

static void a4715_sub(uint32_t *a, const uint32_t *b, int L)
{
    int64_t br = 0;
    int i;
    for (i = 0; i < L; i++) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - br;
        if (s < 0) { s += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)s;
    }
}

/* s = 2^W * arctan(1/m), W = 32*(L-1) */
static void a4715_atan(uint32_t *s, uint32_t m, int L, uint32_t *t, uint32_t *u)
{
    uint32_t k = 0;
    memset(s, 0, sizeof(uint32_t) * L);
    memset(t, 0, sizeof(uint32_t) * L);
    t[L - 1] = 1;
    a4715_divsmall(t, L, m);
    while (!a4715_iszero(t, L)) {
        memcpy(u, t, sizeof(uint32_t) * L);
        a4715_divsmall(u, L, 2 * k + 1);
        if (k % 2 == 0) a4715_add(s, u, L);
        else a4715_sub(s, u, L);
        a4715_divsmall(t, L, m * m);
        k++;
    }
}

static int a4715_cmp(const uint32_t *a, const uint32_t *b, int L)
{
    int i;
    for (i = L - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void a4715_shl(uint32_t *x, int L, int s)
{
    int i;
    for (i = L - 1; i > 0; i--) x[i] = (x[i] << s) | (x[i - 1] >> (32 - s));
    x[0] <<= s;
}

static uint32_t *a4715_pos = NULL;
static idx_t a4715_cnt = 0;

static void a4715_compute(idx_t P)
{
    int L = (int)((P + 96) / 32) + 2;
    uint32_t *s2 = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *s3 = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *t = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *u = (uint32_t *)malloc(sizeof(uint32_t) * L);
    idx_t i, cnt = 0;

    a4715_atan(s2, 2, L, t, u);
    a4715_atan(s3, 3, L, t, u);
    /* t = Pi * 2^W */
    memcpy(t, s2, sizeof(uint32_t) * L);
    a4715_add(t, s3, L);
    a4715_shl(t, L, 2);

    free(a4715_pos);
    a4715_pos = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(P + 1));
    /* r = arctan(1/2) * 2^W, long division by Pi */
    memcpy(u, s2, sizeof(uint32_t) * L);
    for (i = 1; i <= P; i++) {
        a4715_shl(u, L, 1);
        if (a4715_cmp(u, t, L) >= 0) {
            a4715_sub(u, t, L);
            a4715_pos[cnt++] = (uint32_t)i;
        }
    }
    a4715_cnt = cnt;
    free(s2); free(s3); free(t); free(u);
}

seq_t a(idx_t n)
{
    if (n > a4715_cnt) {
        idx_t P = 2 * n + 128;
        for (;;) {
            a4715_compute(P);
            if (a4715_cnt >= n) break;
            P *= 2;
        }
    }
    return (seq_t)a4715_pos[n - 1];
}
