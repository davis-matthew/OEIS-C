/*
 * A064940  Values of k for which A065358(k) is 0.
 * https://oeis.org/A064940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A065358(k) = Sum_{j=1..k} (-1)^pi(j) =
 *            0. Computed by a segmented odd-only prime sieve: between
 *            consecutive primes the ladder moves linearly, so zeros are
 *            located prime-gap by prime-gap
 * Range:     u8 n <= 14, u16 n <= 139, u32 n <= 7730, u64 n <= 11631,
 *            u128 n <= 11631, big any n
 * Validated: standard, pass, 64 terms through n = 7662 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ZR_SEG ((uint64_t)1 << 19)   /* odd numbers per segment */

static idx_t *zr_list = NULL;
static size_t zr_cnt = 0, zr_cap = 0;

static uint64_t zr_cur = 0;      /* all k <= zr_cur processed */
static long long zr_s = 0;       /* A065358(zr_cur) */
static int zr_sgn = 1;           /* (-1)^pi(zr_cur) */
static uint64_t zr_seg_lo = 3;   /* next odd number to sieve */
static int zr_init = 0;

static uint32_t *zr_base = NULL; /* odd base primes */
static size_t zr_base_cnt = 0;
static uint64_t zr_base_lim = 0;
static unsigned char *zr_mark = NULL;

static void zr_push(uint64_t k)
{
    if (zr_cnt == zr_cap) {
        zr_cap = zr_cap ? zr_cap * 2 : 1024;
        zr_list = (idx_t *)realloc(zr_list, zr_cap * sizeof(idx_t));
        if (!zr_list) exit(1);
    }
    zr_list[zr_cnt++] = (idx_t)k;
}

static void zr_build_base(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) exit(1);
    for (uint64_t p = 2; p * p <= lim; ++p)
        if (!c[p])
            for (uint64_t j = p * p; j <= lim; j += p) c[j] = 1;
    size_t cnt = 0;
    for (uint64_t p = 3; p <= lim; p += 2) if (!c[p]) ++cnt;
    free(zr_base);
    zr_base = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    if (!zr_base) exit(1);
    zr_base_cnt = 0;
    for (uint64_t p = 3; p <= lim; p += 2) if (!c[p]) zr_base[zr_base_cnt++] = (uint32_t)p;
    free(c);
    zr_base_lim = lim;
}

static void zr_prime(uint64_t q)
{
    uint64_t t = 0;
    if (zr_sgn < 0 && zr_s > 0) t = (uint64_t)zr_s;
    else if (zr_sgn > 0 && zr_s < 0) t = (uint64_t)(-zr_s);
    if (t >= 1 && zr_cur + t <= q - 1) zr_push(zr_cur + t);
    zr_s += (long long)(q - 1 - zr_cur) * zr_sgn;
    zr_sgn = -zr_sgn;
    zr_s += zr_sgn;
    zr_cur = q;
    if (zr_s == 0) zr_push(q);
}

static void zr_segment(void)
{
    uint64_t lo = zr_seg_lo;
    uint64_t hi = lo + 2 * (ZR_SEG - 1);
    if (zr_base_lim * zr_base_lim < hi) {
        uint64_t r = 1;
        while (r * r < hi) r <<= 1;
        uint64_t nl = zr_base_lim * 2;
        if (nl < r) nl = r;
        zr_build_base(nl);
    }
    if (!zr_mark) {
        zr_mark = (unsigned char *)malloc((size_t)ZR_SEG);
        if (!zr_mark) exit(1);
    }
    memset(zr_mark, 0, (size_t)ZR_SEG);
    for (size_t i = 0; i < zr_base_cnt; ++i) {
        uint64_t p = zr_base[i];
        if (p * p > hi) break;
        uint64_t st = p * p;
        if (st < lo) {
            st = ((lo + p - 1) / p) * p;
            if (!(st & 1)) st += p;
        }
        for (uint64_t j = (st - lo) / 2; j < ZR_SEG; j += p) zr_mark[j] = 1;
    }
    for (uint64_t i = 0; i < ZR_SEG; ++i)
        if (!zr_mark[i]) zr_prime(lo + 2 * i);
    zr_seg_lo = hi + 2;
}

seq_t a(idx_t n)
{
    if (!zr_init) {
        zr_init = 1;
        zr_push(0);
        zr_push(2);
        zr_cur = 2; zr_s = 0; zr_sgn = -1; zr_seg_lo = 3;
    }
    while ((size_t)n > zr_cnt) zr_segment();
    return (seq_t)zr_list[n - 1];
}
