/*
 * A076828  Record high values in A078142.
 * https://oeis.org/A076828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th record value of A078142(m), m = 1, 2, 3, ...;
 *            computed by a segmented sieve over m, where A078142(m) =
 *            Sum_{p|m, p prime} ((floor(sqrt(p))+1)^2 - p)
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9340 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

#define A076828_SEG 65536

static uint32_t *A076828_pr = 0;
static size_t A076828_np = 0;
static uint64_t A076828_plim = 0;

static uint64_t *A076828_rec = 0;
static size_t A076828_cnt = 0, A076828_cap = 0;
static uint64_t A076828_next = 1;
static int64_t A076828_best = -1;

static uint32_t A076828_sum[A076828_SEG];
static uint64_t A076828_prod[A076828_SEG];

static uint64_t A076828_diff(uint64_t p)
{
    uint64_t t = isqrt_u64(p) + 1;
    return t * t - p;
}

static void A076828_ensure_primes(uint64_t r)
{
    uint64_t lim, i, j;
    char *c;
    if (r <= A076828_plim) return;
    lim = r * 2 + 64;
    c = (char *)calloc((size_t)lim + 1, 1);
    free(A076828_pr);
    A076828_pr = (uint32_t *)malloc(((size_t)lim / 2 + 16) * sizeof(uint32_t));
    A076828_np = 0;
    for (i = 2; i <= lim; i++) {
        if (c[i]) continue;
        A076828_pr[A076828_np++] = (uint32_t)i;
        for (j = i * i; j <= lim; j += i) c[j] = 1;
    }
    free(c);
    A076828_plim = lim;
}

static void A076828_push(uint64_t v)
{
    if (A076828_cnt == A076828_cap) {
        A076828_cap = A076828_cap ? A076828_cap * 2 : 1024;
        A076828_rec = (uint64_t *)realloc(A076828_rec, A076828_cap * sizeof(uint64_t));
    }
    A076828_rec[A076828_cnt++] = v;
}

static void A076828_block(void)
{
    const uint64_t B = A076828_SEG;
    uint64_t L = A076828_next, H = L + B - 1;
    uint64_t r = isqrt_u64(H);
    uint64_t i, j, s, T;
    size_t k;

    A076828_ensure_primes(r);
    for (i = 0; i < B; i++) { A076828_sum[i] = 0; A076828_prod[i] = 1; }

    for (k = 0; k < A076828_np; k++) {
        uint64_t p = A076828_pr[k], pk;
        uint32_t d;
        if (p > r) break;
        d = (uint32_t)A076828_diff(p);
        for (j = ((L + p - 1) / p) * p; j <= H; j += p) {
            A076828_sum[j - L] += d;
            A076828_prod[j - L] *= p;
        }
        pk = p * p;
        while (pk <= H) {
            for (j = ((L + pk - 1) / pk) * pk; j <= H; j += pk)
                A076828_prod[j - L] *= p;
            if (pk > H / p) break;
            pk *= p;
        }
    }

    T = 2 * isqrt_u64(H / 2) + 1;   /* bound on diff of a cofactor prime of a composite */
    s = isqrt_u64(L);
    for (i = 0; i < B; i++) {
        uint64_t m = L + i, v;
        while ((s + 1) * (s + 1) <= m) s++;
        if (m == 1) {
            v = 0;
        } else if (A076828_prod[i] == 1) {
            v = (s + 1) * (s + 1) - m;          /* m is prime */
        } else {
            uint64_t q;
            if (A076828_best >= 0 && (int64_t)(A076828_sum[i] + T) <= A076828_best) continue;
            q = m / A076828_prod[i];
            v = A076828_sum[i] + (q > 1 ? A076828_diff(q) : 0);
        }
        if ((int64_t)v > A076828_best) {
            A076828_best = (int64_t)v;
            A076828_push(v);
        }
    }
    A076828_next = H + 1;
}

seq_t a(idx_t n)
{
    while (A076828_cnt < (size_t)n) A076828_block();
    return (seq_t)A076828_rec[n - 1];
}
