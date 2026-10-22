/*
 * A002052  Prime determinants of forms with class number 2.
 * https://oeis.org/A002052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search — a(n) is the n-th prime p == 3 (mod 4) with narrow class
 *            number h+(4p) = 2, i.e. h(Q(sqrt p)) = 1. The test checks that
 *            the number of reduced forms of discriminant 4p equals twice the
 *            length of the principal cycle
 * Range:     u8 n <= 27, u16 n <= 2633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002052_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a002052_isqrt.h"

static uint32_t *a002052_spf = NULL;
static uint64_t a002052_lim = 0;
static uint64_t *a002052_terms = NULL;
static uint64_t a002052_cnt = 0, a002052_cap = 0;
static uint64_t a002052_next = 3;
static uint64_t a002052_div[4096];

static void a002052_sieve(uint64_t lim)
{
    uint64_t i, j;
    free(a002052_spf);
    a002052_spf = (uint32_t *)calloc((size_t)(lim + 1), sizeof(uint32_t));
    for (i = 2; i <= lim; i++) {
        if (a002052_spf[i] == 0) {
            for (j = i; j <= lim; j += i)
                if (a002052_spf[j] == 0) a002052_spf[j] = (uint32_t)i;
        }
    }
    a002052_lim = lim;
}

/* number of divisors d of m with d*d < m and (d+k)^2 > p */
static uint64_t a002052_count(uint64_t m, uint64_t k, uint64_t p)
{
    size_t nd = 1, i, start;
    uint64_t x = m, res = 0;
    a002052_div[0] = 1;
    while (x > 1) {
        uint64_t q = a002052_spf[x], pw = 1;
        size_t base = nd;
        int e = 0;
        while (x % q == 0) { x /= q; e++; }
        for (start = 0; e > 0; e--) {
            pw *= q;
            for (i = 0; i < base; i++) a002052_div[nd++] = a002052_div[i] * pw;
        }
        (void)start;
    }
    for (i = 0; i < nd; i++) {
        uint64_t d = a002052_div[i];
        if (d * d < m && (d + k) * (d + k) > p) res++;
    }
    return res;
}

static int a002052_test(uint64_t p)
{
    long long P = (long long)p;
    long long f = (long long)a002052_isqrt(p);
    long long a, k, c, L = 0;
    uint64_t half = 0, kk;

    /* principal cycle length */
    a = 1; k = f; c = f * f - P;
    do {
        long long ac = c < 0 ? -c : c;
        long long kn = f - ((f + k) % ac);
        long long cn = (kn * kn - P) / c;
        a = c; k = kn; c = cn;
        L++;
    } while (!(a == 1 && k == f));

    /* total number of reduced forms */
    for (kk = 1; kk <= (uint64_t)f; kk++)
        half += a002052_count(p - kk * kk, kk, p);
    return 4 * half == 2 * (uint64_t)L;
}

seq_t a(idx_t n)
{
    while (a002052_cnt < n) {
        uint64_t p = a002052_next;
        if (p > a002052_lim) {
            uint64_t nl = a002052_lim ? 2 * a002052_lim : ((uint64_t)1 << 18);
            while (nl < p) nl *= 2;
            a002052_sieve(nl);
        }
        a002052_next += 4;
        if (a002052_spf[p] != p) continue;
        if (a002052_test(p)) {
            if (a002052_cnt == a002052_cap) {
                a002052_cap = a002052_cap ? 2 * a002052_cap : 1024;
                a002052_terms = (uint64_t *)realloc(a002052_terms, (size_t)a002052_cap * sizeof(uint64_t));
            }
            a002052_terms[a002052_cnt++] = p;
        }
    }
    return (seq_t)a002052_terms[n - 1];
}
