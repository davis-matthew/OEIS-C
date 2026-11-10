/*
 * A064421  Term at which n appears in A064413 (if it begins at 2).
 * https://oeis.org/A064421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-1 where A064413(k) = n (inverse of the EKG sequence,
 *            shifted to start at 0); computed by generating the EKG sequence
 *            lazily with per-prime pointers to the smallest unused multiple
 * Range:     u8 n <= 136, u16 n <= 33789, u32 n <= 33789, u64 n <= 33789,
 *            u128 n <= 33789, big any n
 * Validated: standard, pass, 87 terms through n = 9467 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *ekg_pos = NULL;   /* ekg_pos[v] = 1-based position of v, 0 if unused */
static uint32_t *ekg_ptr = NULL;   /* ekg_ptr[p] = candidate multiple of prime p */
static size_t ekg_cap = 0;
static uint32_t ekg_last = 0;
static uint32_t ekg_len = 0;

static void ekg_ensure(size_t m)
{
    if (m < ekg_cap) return;
    size_t nc = ekg_cap ? ekg_cap : 1024;
    while (nc <= m) nc <<= 1;
    uint32_t *np = (uint32_t *)realloc(ekg_pos, nc * sizeof(uint32_t));
    if (!np) exit(1);
    ekg_pos = np;
    uint32_t *nq = (uint32_t *)realloc(ekg_ptr, nc * sizeof(uint32_t));
    if (!nq) exit(1);
    ekg_ptr = nq;
    memset(ekg_pos + ekg_cap, 0, (nc - ekg_cap) * sizeof(uint32_t));
    memset(ekg_ptr + ekg_cap, 0, (nc - ekg_cap) * sizeof(uint32_t));
    ekg_cap = nc;
}

static void ekg_step(void)
{
    uint32_t x = ekg_last;
    uint32_t best = UINT32_MAX;
    for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
        if (x % p) {
            if ((uint64_t)p * p > x) p = x - 1; /* remaining x is prime; jump */
            continue;
        }
        while (x % p == 0) x /= p;
        ekg_ensure(p + 1);
        uint32_t c = ekg_ptr[p] ? ekg_ptr[p] : p;
        for (;;) {
            ekg_ensure((size_t)c + 1);
            if (!ekg_pos[c]) break;
            c += p;
        }
        ekg_ptr[p] = c;
        if (c < best) best = c;
    }
    ekg_ensure((size_t)best + 1);
    ekg_len++;
    ekg_pos[best] = ekg_len;
    ekg_last = best;
}

seq_t a(idx_t n)
{
    if (ekg_len == 0) {
        ekg_ensure(16);
        ekg_pos[1] = 1;
        ekg_pos[2] = 2;
        ekg_len = 2;
        ekg_last = 2;
    }
    for (;;) {
        if ((size_t)n < ekg_cap && ekg_pos[n]) return (seq_t)(ekg_pos[n] - 1);
        ekg_step();
    }
}
