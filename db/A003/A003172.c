/*
 * A003172  Q(sqrt n) is a unique factorization domain (or simple quadratic
 *          field).
 * https://oeis.org/A003172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search over squarefree d >= 2; let D = d if d == 1 (mod 4),
 *            otherwise D = 4d. d is a term if the (wide) class number of
 *            discriminant D is 1. This holds when the principal cycle of
 *            reduced forms (a>0, a and b with 2a+b > sqrt(D), 2a-b < sqrt(D))
 *            under rho followed by negation contains every reduce...
 * Range:     u8 n <= 83, u16 n <= 9709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3172_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3172_isqrt.h"

static uint32_t *a3172_spf = NULL;
static uint64_t a3172_spf_lim = 0;

static void a3172_ensure(uint64_t lim) {
    uint64_t L, i, j;
    if (lim < a3172_spf_lim) return;
    L = a3172_spf_lim * 2;
    if (L < lim + 1) L = lim + 1;
    if (L < 65536) L = 65536;
    free(a3172_spf);
    a3172_spf = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    for (i = 2; i < L; i++) {
        if (a3172_spf[i] == 0) {
            for (j = i; j < L; j += i)
                if (a3172_spf[j] == 0) a3172_spf[j] = (uint32_t)i;
        }
    }
    a3172_spf_lim = L;
}

/* number of divisors a of m with lo <= a <= hi */
static uint64_t a3172_count_divs(uint64_t m, uint64_t lo, uint64_t hi) {
    uint64_t divs[2048];
    int nd = 1, i, k, e, cur;
    uint64_t cnt = 0;
    divs[0] = 1;
    while (m > 1) {
        uint64_t p = a3172_spf[m], pw = 1;
        e = 0;
        while (m % p == 0) { m /= p; e++; }
        cur = nd;
        for (k = 1; k <= e; k++) {
            pw *= p;
            for (i = 0; i < cur; i++) divs[nd++] = divs[i] * pw;
        }
    }
    for (i = 0; i < nd; i++)
        if (divs[i] >= lo && divs[i] <= hi) cnt++;
    return cnt;
}

static bool a3172_ok(uint64_t d) {
    uint64_t D, s, b, b0, N = 0, L, ca, cb, x;
    int t = 0;
    /* squarefree check and prime count */
    x = d;
    while (x > 1) {
        uint64_t p = a3172_spf[x];
        x /= p;
        if (x % p == 0) return false;
        t++;
    }
    if (d % 4 == 1) D = d;
    else { D = 4 * d; if (d % 2 == 1) t++; }
    if (t >= 3) return false; /* genus theory: h >= 2 */
    s = a3172_isqrt(D);
    for (b = (D & 1) ? 1 : 2; b <= s; b += 2) {
        uint64_t m = (D - b * b) / 4;
        uint64_t lo = (s + 1 - b + 1) / 2;
        uint64_t hi = (s + b) / 2;
        if (lo < 1) lo = 1;
        N += a3172_count_divs(m, lo, hi);
    }
    b0 = ((s & 1) == (D & 1)) ? s : s - 1;
    ca = 1; cb = b0; L = 0;
    do {
        uint64_t C = (D - cb * cb) / (4 * ca);
        uint64_t nb = s - ((s + cb) % (2 * C));
        ca = C; cb = nb;
        L++;
    } while (!(ca == 1 && cb == b0) && L <= N);
    return L == N;
}

static uint64_t *a3172_terms = NULL;
static idx_t a3172_cnt = 0, a3172_cap = 0;
static uint64_t a3172_next = 2;

seq_t a(idx_t n) {
    while (a3172_cnt < n) {
        uint64_t d = a3172_next++;
        a3172_ensure(4 * d + 4);
        if (a3172_ok(d)) {
            if (a3172_cnt == a3172_cap) {
                idx_t nc = a3172_cap ? 2 * a3172_cap : 1024;
                uint64_t *t = (uint64_t *)realloc(a3172_terms, (size_t)nc * sizeof(uint64_t));
                a3172_terms = t;
                a3172_cap = nc;
            }
            a3172_terms[a3172_cnt++] = d;
        }
    }
    return (seq_t)a3172_terms[n - 1];
}
