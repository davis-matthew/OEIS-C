/*
 * A071199  Possible primes appearing in A071196 as certain sums of previous
 *          consecutive primes. See A071196.
 * https://oeis.org/A071199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest distinct value of A071196 (sum of
 *            consecutive primes starting at prime(m), >=2 terms, first prime
 *            sum); computed by search
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A071199_pr = NULL;
static size_t A071199_pn = 0;
static size_t A071199_lim = 0;

static void A071199_ensure(size_t i)
{
    while (A071199_pn <= i) {
        size_t lim = A071199_lim ? A071199_lim * 2 : 4096;
        unsigned char *s = (unsigned char *)calloc(lim + 1, 1);
        if (!s) abort();
        size_t cnt = 0;
        for (size_t x = 2; x <= lim; ++x) {
            if (!s[x]) {
                ++cnt;
                for (size_t y = x * x; y <= lim; y += x) s[y] = 1;
            }
        }
        uint32_t *np = (uint32_t *)realloc(A071199_pr, cnt * sizeof(uint32_t));
        if (!np) abort();
        A071199_pr = np;
        size_t j = 0;
        for (size_t x = 2; x <= lim; ++x) if (!s[x]) np[j++] = (uint32_t)x;
        A071199_pn = cnt;
        A071199_lim = lim;
        free(s);
    }
}

static uint64_t *A071199_vc = NULL;
static size_t A071199_vn = 0;

/* value of A071196(m), m>=1; 0 if none found */
static uint64_t A071199_val(size_t m)
{
    while (A071199_vn < m) {
        size_t idx = A071199_vn;           /* 0-based start */
        uint64_t sum = 0, res = 0;
        for (size_t step = 0; step < 200000; ++step) {
            A071199_ensure(idx + step);
            sum += A071199_pr[idx + step];
            if (step >= 1 && is_prime_u64(sum)) { res = sum; break; }
        }
        uint64_t *nv = (uint64_t *)realloc(A071199_vc, (A071199_vn + 1) * sizeof(uint64_t));
        if (!nv) abort();
        A071199_vc = nv;
        nv[A071199_vn++] = res;
    }
    return A071199_vc[m - 1];
}

static int A071199_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    size_t N = 2 * k + 16;
    for (;;) {
        uint64_t *buf = (uint64_t *)malloc(N * sizeof(uint64_t));
        if (!buf) abort();
        size_t c = 0;
        for (size_t m = 1; m <= N; ++m) {
            uint64_t v = A071199_val(m);
            if (v) buf[c++] = v;
        }
        qsort(buf, c, sizeof(uint64_t), A071199_cmp);
        size_t d = 0;
        for (size_t i = 0; i < c; ++i)
            if (d == 0 || buf[d - 1] != buf[i]) buf[d++] = buf[i];
        if (d >= k) {
            A071199_ensure(N + 1);
            uint64_t bound = 2 * (uint64_t)A071199_pr[N];  /* prime(N+1) */
            if (bound > buf[k - 1]) {
                uint64_t r = buf[k - 1];
                free(buf);
                return (seq_t)r;
            }
        }
        free(buf);
        N *= 2;
    }
}
