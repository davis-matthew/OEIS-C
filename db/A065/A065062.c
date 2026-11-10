/*
 * A065062  Numbers k such that A065608(k) = A065608(k+1).
 * https://oeis.org/A065062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search: k is a term iff A065608(k) = A065608(k+1), where
 *            A065608(m) = sigma(m) - d(m); sigma and d are computed for
 *            consecutive blocks of k with a segmented divisor sieve
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 75 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SEG_SIZE 262144u

static uint64_t *g_primes = NULL;
static size_t g_nprimes = 0;
static uint64_t g_plim = 0;

static void ensure_primes(uint64_t lim) {
    if (lim <= g_plim) return;
    uint64_t L = g_plim * 2;
    if (L < lim) L = lim;
    if (L < 65536) L = 65536;
    unsigned char *c = (unsigned char *)malloc((size_t)L + 1);
    if (!c) return;
    memset(c, 1, (size_t)L + 1);
    c[0] = 0; c[1] = 0;
    for (uint64_t i = 2; i * i <= L; i++)
        if (c[i]) for (uint64_t j = i * i; j <= L; j += i) c[j] = 0;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= L; i++) if (c[i]) cnt++;
    uint64_t *p = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    if (!p) { free(c); return; }
    size_t k = 0;
    for (uint64_t i = 2; i <= L; i++) if (c[i]) p[k++] = i;
    free(c);
    free(g_primes);
    g_primes = p;
    g_nprimes = cnt;
    g_plim = L;
}

static uint64_t *g_terms = NULL;
static size_t g_count = 0;
static size_t g_cap = 0;
static uint64_t g_next = 1;      /* next block starts here */
static uint64_t g_prevf = 0;     /* f(g_next - 1), valid when g_next > 1 */

static uint64_t *g_rem = NULL, *g_sig = NULL, *g_f = NULL;
static uint32_t *g_tau = NULL;

static void push_term(uint64_t k) {
    if (g_count == g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(g_terms, nc * sizeof(uint64_t));
        if (!t) return;
        g_terms = t;
        g_cap = nc;
    }
    g_terms[g_count++] = k;
}

static bool process_segment(void) {
    if (!g_rem) {
        g_rem = (uint64_t *)malloc(SEG_SIZE * sizeof(uint64_t));
        g_sig = (uint64_t *)malloc(SEG_SIZE * sizeof(uint64_t));
        g_f   = (uint64_t *)malloc(SEG_SIZE * sizeof(uint64_t));
        g_tau = (uint32_t *)malloc(SEG_SIZE * sizeof(uint32_t));
        if (!g_rem || !g_sig || !g_f || !g_tau) return false;
    }
    uint64_t lo = g_next, hi = lo + SEG_SIZE - 1;
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= hi) r++;
    ensure_primes(r);
    if (g_plim < r) return false;

    for (uint64_t i = 0; i < SEG_SIZE; i++) {
        g_rem[i] = lo + i; g_sig[i] = 1; g_tau[i] = 1;
    }
    for (size_t pi = 0; pi < g_nprimes; pi++) {
        uint64_t p = g_primes[pi];
        if (p * p > hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t m = start; m <= hi; m += p) {
            uint64_t i = m - lo;
            uint64_t x = g_rem[i], pk = 1, s = 1;
            uint32_t e = 0;
            do { x /= p; pk *= p; s += pk; e++; } while (x % p == 0);
            g_rem[i] = x;
            g_sig[i] *= s;
            g_tau[i] *= (e + 1);
        }
    }
    for (uint64_t i = 0; i < SEG_SIZE; i++) {
        if (g_rem[i] > 1) { g_sig[i] *= (g_rem[i] + 1); g_tau[i] *= 2; }
        g_f[i] = g_sig[i] - g_tau[i];
    }
    if (lo > 1 && g_prevf == g_f[0]) push_term(lo - 1);
    for (uint64_t i = 0; i + 1 < SEG_SIZE; i++)
        if (g_f[i] == g_f[i + 1]) push_term(lo + i);
    g_prevf = g_f[SEG_SIZE - 1];
    g_next = hi + 1;
    return true;
}

seq_t a(idx_t n) {
    while (g_count < (size_t)n) {
        if (!process_segment()) return 0;
    }
    return (seq_t)g_terms[(size_t)n - 1];
}
