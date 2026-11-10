/*
 * A064952  Number of odd terms among first n terms of A064413.
 * https://oeis.org/A064952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (A064413(k) mod 2), where A064413 is the EKG
 *            sequence (a(1)=1, a(2)=2, then smallest unused number sharing a
 *            factor with the previous term); computed by direct simulation
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static unsigned char *ekg_used = NULL;
static uint32_t *ekg_ptr = NULL;     /* ekg_ptr[p]: last multiple of p checked */
static size_t ekg_cap = 0;
static uint32_t *ekg_odd = NULL;     /* prefix counts of odd terms, 1-based */
static size_t ekg_odd_cap = 0;
static size_t ekg_len = 0;
static uint64_t ekg_last = 0;

static void ekg_ensure(size_t v)
{
    if (v < ekg_cap) return;
    size_t nc = ekg_cap ? ekg_cap : 1024;
    while (nc <= v) nc <<= 1;
    unsigned char *nu = (unsigned char *)realloc(ekg_used, nc);
    uint32_t *np = (uint32_t *)realloc(ekg_ptr, nc * sizeof(uint32_t));
    if (!nu || !np) exit(1);
    memset(nu + ekg_cap, 0, nc - ekg_cap);
    memset(np + ekg_cap, 0, (nc - ekg_cap) * sizeof(uint32_t));
    ekg_used = nu;
    ekg_ptr = np;
    ekg_cap = nc;
}

static void ekg_push(uint64_t v)
{
    if (ekg_len + 2 > ekg_odd_cap) {
        size_t nc = ekg_odd_cap ? ekg_odd_cap * 2 : 1024;
        uint32_t *t = (uint32_t *)realloc(ekg_odd, nc * sizeof(uint32_t));
        if (!t) exit(1);
        ekg_odd = t;
        ekg_odd_cap = nc;
    }
    ekg_odd[ekg_len + 1] = ekg_odd[ekg_len] + (uint32_t)(v & 1);
    ekg_len++;
    ekg_last = v;
}

static void ekg_next(void)
{
    uint64_t x = ekg_last;
    uint64_t best = 0;
    uint64_t p;
    for (p = 2; p * p <= x || x > 1; p++) {
        if (x % p != 0) {
            if (p * p > x) p = x - 1; /* remaining x is prime; jump to it */
            continue;
        }
        while (x % p == 0) x /= p;
        ekg_ensure((size_t)p);
        uint64_t c = ekg_ptr[p] ? ekg_ptr[p] : p;
        for (;;) {
            ekg_ensure((size_t)c);
            if (!ekg_used[c]) break;
            c += p;
        }
        ekg_ptr[p] = (uint32_t)c;
        if (best == 0 || c < best) best = c;
        if (x == 1) break;
    }
    ekg_ensure((size_t)best);
    ekg_used[best] = 1;
    ekg_push(best);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (ekg_len == 0) {
        ekg_ensure(1024);
        ekg_odd = NULL;
        ekg_odd_cap = 0;
        ekg_push(1);
        ekg_used[1] = 1;
        ekg_push(2);
        ekg_used[2] = 1;
    }
    while ((size_t)n > ekg_len) ekg_next();
    return (seq_t)ekg_odd[(size_t)n];
}
