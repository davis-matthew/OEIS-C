/*
 * A095299  Numbers n such that A094757(n) < n.
 * https://oeis.org/A095299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve. m is in the sequence iff some k<m has pi(k)*m =
 *            k*pi(m), i.e. pi(t*m/g)=t*pi(m)/g for some 1<=t<g,
 *            g=gcd(m,pi(m))
 * Range:     u8 n <= 21, u16 n <= 231, u32 n <= 1145, u64 n <= 1145,
 *            u128 n <= 1145, big any n
 * Validated: standard, pass, 68 terms through n = 1112 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A095299_terms = NULL;
static uint64_t A095299_cnt = 0;

static uint64_t A095299_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A095299_build(uint64_t limit)
{
    uint32_t *pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    uint64_t *out = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    uint64_t c = 0, i, j;
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    pi[0] = 0;
    if (limit >= 1) pi[1] = 0;
    for (i = 2; i <= limit; i++) pi[i] = pi[i - 1] + (comp[i] ? 0 : 1);
    for (i = 2; i <= limit; i++) {
        uint64_t p = pi[i];
        uint64_t g = A095299_gcd(i, p);
        uint64_t d = i / g, q = p / g, t;
        bool found = false;
        for (t = 1; t < g; t++) {
            if ((uint64_t)pi[t * d] == t * q) { found = true; break; }
        }
        if (found) out[c++] = i;
    }
    free(pi);
    free(comp);
    free(A095299_terms);
    A095299_terms = out;
    A095299_cnt = c;
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n < 1) return (seq_t)0;
    if (limit == 0) { limit = 1 << 14; A095299_build(limit); }
    while ((uint64_t)n > A095299_cnt) {
        limit *= 2;
        A095299_build(limit);
    }
    return (seq_t)A095299_terms[n - 1];
}
