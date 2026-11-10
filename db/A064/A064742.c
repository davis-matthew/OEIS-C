/*
 * A064742  Total number of primes (with multiplicity) dividing A064413(n).
 * https://oeis.org/A064742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(A064413(n)), where A064413 is the EKG sequence:
 *            a(1)=1, a(2)=2, a(n) = smallest unused number sharing a factor
 *            with a(n-1) (computed by sieve-like search)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8918 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *ekg_terms = NULL;
static size_t ekg_len = 0, ekg_tcap = 0;
static unsigned char *ekg_used = NULL;
static uint32_t *ekg_ptr = NULL;
static size_t ekg_cap = 0;

static void ekg_grow(size_t need)
{
    if (need < ekg_cap) return;
    size_t nc = ekg_cap ? ekg_cap : 1024;
    while (nc <= need) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(ekg_used, nc);
    uint32_t *p = (uint32_t *)realloc(ekg_ptr, nc * sizeof(uint32_t));
    if (!u || !p) exit(1);
    memset(u + ekg_cap, 0, nc - ekg_cap);
    memset(p + ekg_cap, 0, (nc - ekg_cap) * sizeof(uint32_t));
    ekg_used = u;
    ekg_ptr = p;
    ekg_cap = nc;
}

static void ekg_push(uint32_t v)
{
    if (ekg_len + 1 >= ekg_tcap) {
        size_t nc = ekg_tcap ? ekg_tcap * 2 : 1024;
        uint32_t *t = (uint32_t *)realloc(ekg_terms, nc * sizeof(uint32_t));
        if (!t) exit(1);
        ekg_terms = t;
        ekg_tcap = nc;
    }
    ekg_terms[++ekg_len] = v;
}

static void ekg_extend(size_t n)
{
    if (ekg_len == 0) {
        ekg_grow(16);
        ekg_push(1);
        ekg_push(2);
        ekg_used[1] = 1;
        ekg_used[2] = 1;
    }
    while (ekg_len < n) {
        uint32_t prev = ekg_terms[ekg_len];
        uint32_t x = prev, best = 0;
        for (uint32_t p = 2; x > 1; ++p) {
            if ((uint64_t)p * p > x) p = x;
            if (x % p) continue;
            while (x % p == 0) x /= p;
            ekg_grow(p);
            uint32_t m = ekg_ptr[p] ? ekg_ptr[p] : p;
            for (;;) {
                ekg_grow(m);
                if (!ekg_used[m]) break;
                m += p;
            }
            ekg_ptr[p] = m;
            if (best == 0 || m < best) best = m;
        }
        ekg_grow(best);
        ekg_used[best] = 1;
        ekg_push(best);
    }
}

seq_t a(idx_t n)
{
    ekg_extend((size_t)n);
    uint32_t t = ekg_terms[n];
    unsigned c = 0;
    for (uint32_t p = 2; (uint64_t)p * p <= t; ++p) {
        while (t % p == 0) { t /= p; ++c; }
    }
    if (t > 1) ++c;
    return (seq_t)c;
}
