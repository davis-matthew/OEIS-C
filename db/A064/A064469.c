/*
 * A064469  The even subsequence of A064413.
 * https://oeis.org/A064469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th even term of the EKG sequence A064413 (A064413(1)=1,
 *            A064413(2)=2, then smallest unused number sharing a factor with
 *            the previous term); computed by simulation
 * Range:     u8 n <= 127, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 76 terms through n = 9488 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static unsigned char *ekg_used = NULL;
static uint32_t *ekg_nxt = NULL;
static size_t ekg_cap = 0;
static uint32_t ekg_prev = 0;
static uint32_t *ekg_evens = NULL;
static size_t ekg_ev_len = 0, ekg_ev_cap = 0;

static void ekg_grow(size_t nc)
{
    size_t c = ekg_cap ? ekg_cap : 1024;
    while (c <= nc) c *= 2;
    unsigned char *u = (unsigned char *)realloc(ekg_used, c);
    uint32_t *x = (uint32_t *)realloc(ekg_nxt, c * sizeof(uint32_t));
    if (!u || !x) exit(1);
    memset(u + ekg_cap, 0, c - ekg_cap);
    memset(x + ekg_cap, 0, (c - ekg_cap) * sizeof(uint32_t));
    ekg_used = u;
    ekg_nxt = x;
    ekg_cap = c;
}

static void ekg_push_even(uint32_t v)
{
    if (ekg_ev_len == ekg_ev_cap) {
        size_t nc = ekg_ev_cap ? ekg_ev_cap * 2 : 1024;
        uint32_t *e = (uint32_t *)realloc(ekg_evens, nc * sizeof(uint32_t));
        if (!e) exit(1);
        ekg_evens = e;
        ekg_ev_cap = nc;
    }
    ekg_evens[ekg_ev_len++] = v;
}

static void ekg_step(void)
{
    if (ekg_prev == 0) {
        ekg_grow(4096);
        ekg_used[1] = 1;
        ekg_used[2] = 1;
        ekg_prev = 2;
        ekg_push_even(2);
        return;
    }
    uint32_t x = ekg_prev;
    uint32_t best = 0xFFFFFFFFu;
    for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
        if (x == 1) break;
        if ((uint64_t)p * p > x) p = x;
        if (x % p) continue;
        while (x % p == 0) x /= p;
        if (p >= ekg_cap) ekg_grow(p);
        uint32_t c = ekg_nxt[p] ? ekg_nxt[p] : p;
        for (;;) {
            if (c >= ekg_cap) ekg_grow(c);
            if (!ekg_used[c]) break;
            c += p;
        }
        ekg_nxt[p] = c;
        if (c < best) best = c;
    }
    ekg_used[best] = 1;
    ekg_prev = best;
    if ((best & 1u) == 0) ekg_push_even(best);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while ((idx_t)ekg_ev_len < n) ekg_step();
    return (seq_t)ekg_evens[n - 1];
}
