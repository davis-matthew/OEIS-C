/*
 * A065519  a(n) = A064413(n)-n.
 * https://oeis.org/A065519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A064413(n) - n, where A064413 is the EKG sequence:
 *            A064413(1)=1, A064413(2)=2, A064413(n) = smallest unused number
 *            sharing a factor with A064413(n-1); computed by direct
 *            simulation
 * Range:     i8 n <= 236, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9931 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *ekg_t = NULL;
static size_t ekg_len = 0, ekg_cap = 0;
static unsigned char *ekg_used = NULL;
static uint64_t *ekg_nxt = NULL;
static size_t ekg_ucap = 0;

static void ekg_grow(size_t m)
{
    if (m < ekg_ucap) return;
    size_t nc = ekg_ucap ? ekg_ucap : 1024;
    while (nc <= m) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(ekg_used, nc);
    uint64_t *x = (uint64_t *)realloc(ekg_nxt, nc * sizeof(uint64_t));
    if (!u || !x) exit(1);
    memset(u + ekg_ucap, 0, nc - ekg_ucap);
    memset(x + ekg_ucap, 0, (nc - ekg_ucap) * sizeof(uint64_t));
    ekg_used = u;
    ekg_nxt = x;
    ekg_ucap = nc;
}

static void ekg_push(uint64_t v)
{
    if (ekg_len >= ekg_cap) {
        size_t nc = ekg_cap ? ekg_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(ekg_t, nc * sizeof(uint64_t));
        if (!t) exit(1);
        ekg_t = t;
        ekg_cap = nc;
    }
    ekg_t[ekg_len++] = v;
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
        uint64_t prev = ekg_t[ekg_len - 1];
        uint64_t x = prev, best = 0;
        for (uint64_t p = 2; p * p <= x || x > 1; p++) {
            uint64_t q;
            if (p * p > x) q = x; else {
                if (x % p) continue;
                q = p;
            }
            while (x % q == 0) x /= q;
            /* q is a prime factor */
            uint64_t c = ekg_nxt[q < ekg_ucap ? q : 0];
            if (q >= ekg_ucap) { ekg_grow((size_t)q); c = ekg_nxt[q]; }
            if (c == 0) c = q;
            for (;;) {
                if (c >= ekg_ucap) ekg_grow((size_t)c);
                if (!ekg_used[c]) break;
                c += q;
            }
            ekg_nxt[q] = c;
            if (best == 0 || c < best) best = c;
            if (x == 1) break;
        }
        ekg_grow((size_t)best);
        ekg_used[best] = 1;
        ekg_push(best);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ekg_extend((size_t)n);
    return (seq_t)ekg_t[n - 1] - (seq_t)n;
}
