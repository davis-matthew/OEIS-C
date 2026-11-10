/*
 * A064472  Where the even terms appear in A064413 (if it begins with 2).
 * https://oeis.org/A064472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-1 where k is the position of the n-th even term of the
 *            EKG sequence A064413 (a(1)=1, a(2)=2, then smallest unused
 *            number sharing a factor with the previous term); computed by
 *            direct simulation
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9083 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *ekg_used_nm = NULL;   /* next candidate multiple per prime */
static unsigned char *ekg_used = NULL; /* used flags by value */
static size_t ekg_cap = 0;             /* capacity of value arrays */
static uint32_t *ekg_pos = NULL;       /* positions (1-based) of even terms */
static size_t ekg_pos_cnt = 0, ekg_pos_cap = 0;
static uint32_t ekg_prev = 0;
static uint32_t ekg_len = 0;

static void ekg_ensure(size_t v)
{
    if (v < ekg_cap) return;
    size_t nc = ekg_cap ? ekg_cap : 1024;
    while (nc <= v) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(ekg_used, nc);
    uint32_t *m = (uint32_t *)realloc(ekg_used_nm, nc * sizeof(uint32_t));
    if (!u || !m) exit(1);
    memset(u + ekg_cap, 0, nc - ekg_cap);
    memset(m + ekg_cap, 0, (nc - ekg_cap) * sizeof(uint32_t));
    ekg_used = u;
    ekg_used_nm = m;
    ekg_cap = nc;
}

static void ekg_push(uint32_t v)
{
    ekg_len++;
    ekg_ensure(v);
    ekg_used[v] = 1;
    ekg_prev = v;
    if ((v & 1u) == 0) {
        if (ekg_pos_cnt == ekg_pos_cap) {
            ekg_pos_cap = ekg_pos_cap ? ekg_pos_cap * 2 : 1024;
            ekg_pos = (uint32_t *)realloc(ekg_pos, ekg_pos_cap * sizeof(uint32_t));
            if (!ekg_pos) exit(1);
        }
        ekg_pos[ekg_pos_cnt++] = ekg_len;
    }
}

static void ekg_step(void)
{
    uint32_t x = ekg_prev;
    uint32_t best = 0xFFFFFFFFu;
    for (uint32_t p = 2; x > 1; ++p) {
        if ((uint64_t)p * p > x) p = x;
        if (x % p) continue;
        while (x % p == 0) x /= p;
        ekg_ensure(p);
        uint32_t c = ekg_used_nm[p] ? ekg_used_nm[p] : p;
        for (;;) {
            ekg_ensure(c);
            if (!ekg_used[c]) break;
            c += p;
        }
        ekg_used_nm[p] = c;
        if (c < best) best = c;
    }
    ekg_push(best);
}

seq_t a(idx_t n)
{
    if (ekg_len == 0) {
        ekg_push(1);
        ekg_push(2);
    }
    while ((idx_t)ekg_pos_cnt < n) ekg_step();
    return (seq_t)(ekg_pos[n - 1] - 1);
}
