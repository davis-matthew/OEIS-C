/*
 * A089173  Solutions x for x^4-1 = kp where p is prime and k=1,2...
 * https://oeis.org/A089173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 990 (2026-11-02)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A089173, as the Maple program of the entry defines it: for k = 2, 3, ...,
   the residues 0 < x < k with x^4 == 1 (mod k) for which (x^4 - 1)/k is
   prime, in increasing order.

   Each such pair has k = (x^4 - 1)/p for a prime p dividing
   x^4 - 1 = (x - 1)(x + 1)(x^2 + 1), so p <= x^2 + 1 and k >= x^2 - 1. All
   pairs with k <= K therefore have x <= sqrt(K + 1): they are found by
   factoring x^4 - 1 for those x, and sorted by (k, x). K doubles until the
   pairs with k <= K hold the term asked for. */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t k, x; } pair_t;

static uint64_t *terms = NULL;
static size_t term_len = 0;

static int cmp_pair(const void *u, const void *v)
{
    const pair_t *a = (const pair_t *)u, *b = (const pair_t *)v;
    if (a->k != b->k) return a->k < b->k ? -1 : 1;
    return (a->x > b->x) - (a->x < b->x);
}

/* the distinct prime factors of m, appended to f (trial division) */
static int add_factors(uint64_t m, uint64_t *f, int c)
{
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d) continue;
        int seen = 0;
        for (int i = 0; i < c; ++i) seen |= f[i] == d;
        if (!seen) f[c++] = d;
        while (m % d == 0) m /= d;
    }
    if (m > 1) {
        int seen = 0;
        for (int i = 0; i < c; ++i) seen |= f[i] == m;
        if (!seen) f[c++] = m;
    }
    return c;
}

static void build(uint64_t K)
{
    size_t cap = 1024, len = 0;
    pair_t *P = (pair_t *)malloc(cap * sizeof *P);
    if (!P) abort();
    for (uint64_t x = 2; x * x <= K + 1; ++x) {
        uint64_t f[64];
        int c = add_factors(x - 1, f, 0);
        c = add_factors(x + 1, f, c);
        c = add_factors(x * x + 1, f, c);
        const uint64_t x4m1 = (x * x - 1) * (x * x + 1);     /* x < 2^16 + 1: fits */
        for (int i = 0; i < c; ++i) {
            uint64_t k = x4m1 / f[i];
            if (k <= x || k > K) continue;
            if (len == cap) {
                pair_t *t = (pair_t *)realloc(P, (cap *= 2) * sizeof *P);
                if (!t) abort();
                P = t;
            }
            P[len].k = k;
            P[len].x = x;
            ++len;
        }
    }
    qsort(P, len, sizeof *P, cmp_pair);
    free(terms);
    terms = (uint64_t *)malloc((len ? len : 1) * sizeof *terms);
    if (!terms) abort();
    for (size_t i = 0; i < len; ++i) terms[i] = P[i].x;
    term_len = len;
    free(P);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    for (uint64_t K = UINT64_C(1) << 16; term_len < (size_t)n; K *= 2) {
        if (K > (UINT64_C(1) << 32)) return (seq_t)0;      /* x^4 - 1 would not fit */
        build(K);
    }
    return (seq_t)terms[n - 1];
}
