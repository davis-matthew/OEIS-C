/*
 * A064026  Numbers k such that d(k) + d(k+1) + d(k+2) = 8, where d(k) =
 *          A001223.
 * https://oeis.org/A064026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search -- a(n) = n-th k with prime(k+3) - prime(k) = 8 (sum of
 *            three consecutive prime gaps A001223 equals 8), found by an
 *            incremental segmented odd-only sieve of Eratosthenes that counts
 *            primes
 * Range:     u8 n <= 7, u16 n <= 144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9984 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A064026_SEG 262144u

static uint32_t *a064026_bp = NULL;
static size_t a064026_nbp = 0;

static uint64_t *a064026_res = NULL;
static size_t a064026_cnt = 0;
static size_t a064026_cap = 0;

static uint64_t a064026_lo = 3;      /* next odd number to sieve */
static uint64_t a064026_pidx = 1;    /* primes counted so far (2 already) */
static uint64_t a064026_w[4] = {0, 0, 0, 2};
static uint64_t a064026_filled = 1;
static unsigned char *a064026_seg = NULL;

static int a064026_init(void)
{
    if (a064026_bp) return 1;
    const uint32_t L = 65536;
    unsigned char *c = (unsigned char *)malloc(L + 1);
    if (!c) return 0;
    memset(c, 0, L + 1);
    uint32_t i, j;
    size_t cnt = 0;
    for (i = 3; i <= L; i += 2) {
        if (!c[i]) {
            cnt++;
            for (j = i * i; j <= L; j += 2 * i) c[j] = 1;
        }
    }
    a064026_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!a064026_bp) { free(c); return 0; }
    size_t k = 0;
    for (i = 3; i <= L; i += 2) if (!c[i]) a064026_bp[k++] = i;
    a064026_nbp = k;
    free(c);
    a064026_seg = (unsigned char *)malloc(A064026_SEG);
    if (!a064026_seg) return 0;
    return 1;
}

static int a064026_push(uint64_t v)
{
    if (a064026_cnt == a064026_cap) {
        size_t nc = a064026_cap ? a064026_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(a064026_res, nc * sizeof(uint64_t));
        if (!t) return 0;
        a064026_res = t;
        a064026_cap = nc;
    }
    a064026_res[a064026_cnt++] = v;
    return 1;
}

static void a064026_extend(size_t need)
{
    if (!a064026_init()) return;
    unsigned char *seg = a064026_seg;
    while (a064026_cnt < need) {
        uint64_t lo = a064026_lo;
        uint64_t hi = lo + 2ull * A064026_SEG; /* exclusive */
        memset(seg, 0, A064026_SEG);
        size_t t;
        for (t = 0; t < a064026_nbp; t++) {
            uint64_t p = a064026_bp[t];
            if (p * p >= hi) break;
            uint64_t s = p * p;
            if (s < lo) {
                s = ((lo + p - 1) / p) * p;
                if ((s & 1) == 0) s += p;
            }
            uint64_t step = 2 * p;
            uint64_t idx;
            for (idx = (s - lo) >> 1; idx < A064026_SEG; idx += p) seg[idx] = 1;
            (void)step;
        }
        uint32_t i;
        for (i = 0; i < A064026_SEG; i++) {
            if (seg[i]) continue;
            uint64_t v = lo + 2ull * i;
            a064026_pidx++;
            a064026_w[0] = a064026_w[1];
            a064026_w[1] = a064026_w[2];
            a064026_w[2] = a064026_w[3];
            a064026_w[3] = v;
            a064026_filled++;
            if (a064026_filled >= 4 && a064026_w[3] - a064026_w[0] == 8) {
                if (!a064026_push(a064026_pidx - 3)) return;
            }
        }
        a064026_lo = hi;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t need = (size_t)n;
    if (a064026_cnt < need) a064026_extend(need);
    if (a064026_cnt < need) return (seq_t)0;
    return (seq_t)a064026_res[need - 1];
}
