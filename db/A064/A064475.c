/*
 * A064475  Number of odd terms among first n terms of A064413 (if it begins
 *          at 2).
 * https://oeis.org/A064475
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd terms among b(2..n), where b is the EKG
 *            sequence A064413 (b(1)=1, b(2)=2, b(n)=smallest unused number
 *            sharing a factor with b(n-1)); computed by direct simulation
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *ekg_t = NULL;
static uint32_t *ekg_cum = NULL;
static size_t ekg_len = 0, ekg_cap = 0;
static unsigned char *ekg_used = NULL;
static uint64_t *ekg_ptr = NULL;
static size_t ekg_vcap = 0;

static void ekg_grow_vals(size_t need)
{
    size_t nc = ekg_vcap ? ekg_vcap : 1024;
    while (nc <= need) nc *= 2;
    ekg_used = (unsigned char *)realloc(ekg_used, nc);
    ekg_ptr = (uint64_t *)realloc(ekg_ptr, nc * sizeof(uint64_t));
    if (!ekg_used || !ekg_ptr) exit(1);
    memset(ekg_used + ekg_vcap, 0, nc - ekg_vcap);
    memset(ekg_ptr + ekg_vcap, 0, (nc - ekg_vcap) * sizeof(uint64_t));
    ekg_vcap = nc;
}

static void ekg_push(uint64_t v)
{
    if (ekg_len + 1 >= ekg_cap) {
        size_t nc = ekg_cap ? ekg_cap * 2 : 1024;
        ekg_t = (uint64_t *)realloc(ekg_t, nc * sizeof(uint64_t));
        ekg_cum = (uint32_t *)realloc(ekg_cum, nc * sizeof(uint32_t));
        if (!ekg_t || !ekg_cum) exit(1);
        ekg_cap = nc;
    }
    ekg_len++;
    ekg_t[ekg_len] = v;
    ekg_cum[ekg_len] = (ekg_len >= 2 ? ekg_cum[ekg_len - 1] : 0) + ((ekg_len >= 2 && (v & 1)) ? 1u : 0u);
}

static void ekg_init(void)
{
    ekg_grow_vals(4096);
    ekg_push(1);
    ekg_push(2);
    ekg_used[1] = 1;
    ekg_used[2] = 1;
}

static void ekg_next(void)
{
    uint64_t x = ekg_t[ekg_len];
    uint64_t best = 0;
    for (uint64_t p = 2; p * p <= x || x > 1; p++) {
        if (p * p > x) p = x;
        if (x % p) continue;
        while (x % p == 0) x /= p;
        uint64_t c = ekg_ptr[p] ? ekg_ptr[p] : p;
        for (;;) {
            while (c >= ekg_vcap) ekg_grow_vals((size_t)c);
            if (!ekg_used[c]) break;
            c += p;
        }
        ekg_ptr[p] = c;
        if (best == 0 || c < best) best = c;
        if (x == 1) break;
    }
    while (best >= ekg_vcap) ekg_grow_vals((size_t)best);
    ekg_used[best] = 1;
    ekg_push(best);
}

seq_t a(idx_t n)
{
    if (ekg_len == 0) ekg_init();
    if (n < 2) return (seq_t)0;
    while ((idx_t)ekg_len < n) ekg_next();
    return (seq_t)ekg_cum[(size_t)n];
}
