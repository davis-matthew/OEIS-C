/*
 * A200145  Primitive (squarefree) elements of A199745.
 * https://oeis.org/A200145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, via a segmented sieve, for squarefree x with omega(x) >=
 *            3 such that minpf(x) + maxpf(x) = sum of the other distinct
 *            prime factors of x
 * Range:     u8 none, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9963 (2026-11-07)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/isqrt_u64.h"

#define A200145_B 65536

static seq_t *A200145_cache = NULL;
static size_t A200145_len = 0;
static size_t A200145_cap = 0;
static uint64_t A200145_lo = 2;

static uint32_t *A200145_pl = NULL;
static size_t A200145_pc = 0;
static uint64_t A200145_plim = 0;

static uint64_t A200145_rem[A200145_B];
static uint64_t A200145_sum[A200145_B];
static uint64_t A200145_mn[A200145_B];
static uint64_t A200145_mx[A200145_B];
static unsigned char A200145_cnt[A200145_B];
static unsigned char A200145_bad[A200145_B];

static void A200145_append(seq_t v)
{
    if (A200145_len >= A200145_cap) {
        size_t nc = A200145_cap ? A200145_cap * 2 : 1024;
        A200145_cache = mem_resize(seq_t, A200145_cache, nc);
        A200145_cap = nc;
    }
    A200145_cache[A200145_len++] = v;
}

static void A200145_ensure_primes(uint64_t need)
{
    uint64_t lim, i, j;
    unsigned char *comp;
    size_t c = 0;
    if (A200145_plim >= need) return;
    lim = need * 2;
    if (lim < 4096) lim = 4096;
    comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    for (i = 2; i * i <= lim; ++i)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    for (i = 2; i <= lim; ++i) if (!comp[i]) ++c;
    free(A200145_pl);
    A200145_pl = (uint32_t *)malloc(c * sizeof(uint32_t));
    c = 0;
    for (i = 2; i <= lim; ++i) if (!comp[i]) A200145_pl[c++] = (uint32_t)i;
    A200145_pc = c;
    A200145_plim = lim;
    free(comp);
}

static void A200145_block(void)
{
    uint64_t lo = A200145_lo, hi = lo + A200145_B;
    uint64_t r = isqrt_u64(hi) + 1;
    size_t k, i;
    A200145_ensure_primes(r);
    for (i = 0; i < A200145_B; ++i) {
        A200145_rem[i] = lo + i;
        A200145_sum[i] = 0;
        A200145_mn[i] = 0;
        A200145_mx[i] = 0;
        A200145_cnt[i] = 0;
        A200145_bad[i] = 0;
    }
    for (k = 0; k < A200145_pc; ++k) {
        uint64_t p = A200145_pl[k], m;
        if (p * p >= hi) break;
        m = ((lo + p - 1) / p) * p;
        for (; m < hi; m += p) {
            i = (size_t)(m - lo);
            if (A200145_bad[i]) continue;
            A200145_rem[i] /= p;
            if (A200145_rem[i] % p == 0) { A200145_bad[i] = 1; continue; }
            if (A200145_cnt[i] == 0) A200145_mn[i] = p;
            A200145_mx[i] = p;
            A200145_sum[i] += p;
            A200145_cnt[i]++;
        }
    }
    for (i = 0; i < A200145_B; ++i) {
        uint64_t sm, mn, mx;
        if (A200145_bad[i]) continue;
        if (A200145_rem[i] > 1) {
            if (A200145_cnt[i] == 0) continue; /* prime */
            A200145_mx[i] = A200145_rem[i];
            A200145_sum[i] += A200145_rem[i];
            A200145_cnt[i]++;
        }
        if (A200145_cnt[i] < 3) continue;
        mn = A200145_mn[i];
        mx = A200145_mx[i];
        sm = A200145_sum[i];
        if (mn + mx == sm - mn - mx)
            A200145_append((seq_t)(lo + i));
    }
    A200145_lo = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A200145_len < (size_t)n)
        A200145_block();
    return A200145_cache[n - 1];
}
