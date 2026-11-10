/*
 * A064667  Regard A064413 as giving a permutation of the positive integers;
 *          sequence gives second infinite cycle, beginning at its smallest
 *          term, 73.
 * https://oeis.org/A064667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   iterate the EKG permutation A064413 from x = 73: a(1) = 73,
 *            a(n+1) = A064413(a(n)); the EKG terms are found by a lazy
 *            sieve-based simulation
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *ek_seq = NULL;   /* EKG terms, 1-based */
static size_t ek_len = 0, ek_cap = 0;
static unsigned char *ek_used = NULL;
static uint32_t *ek_ptr = NULL;   /* per prime: current candidate multiple */
static uint32_t *ek_spf = NULL;
static size_t ek_vcap = 0;        /* values handled are < ek_vcap */

static void ek_grow_values(size_t need)
{
    size_t nc = ek_vcap ? ek_vcap : 1024;
    while (nc <= need) nc *= 2;
    ek_used = (unsigned char *)realloc(ek_used, nc);
    ek_ptr = (uint32_t *)realloc(ek_ptr, nc * sizeof(uint32_t));
    ek_spf = (uint32_t *)realloc(ek_spf, nc * sizeof(uint32_t));
    if (!ek_used || !ek_ptr || !ek_spf) exit(1);
    memset(ek_used + ek_vcap, 0, nc - ek_vcap);
    memset(ek_ptr + ek_vcap, 0, (nc - ek_vcap) * sizeof(uint32_t));
    ek_vcap = nc;
    memset(ek_spf, 0, nc * sizeof(uint32_t));
    for (size_t i = 2; i < nc; i++) {
        if (ek_spf[i] == 0) {
            for (size_t j = i; j < nc; j += i)
                if (ek_spf[j] == 0) ek_spf[j] = (uint32_t)i;
        }
    }
}

static void ek_push(uint32_t v)
{
    if (ek_len + 1 >= ek_cap) {
        ek_cap = ek_cap ? ek_cap * 2 : 1024;
        ek_seq = (uint32_t *)realloc(ek_seq, ek_cap * sizeof(uint32_t));
        if (!ek_seq) exit(1);
    }
    ek_seq[++ek_len] = v;
}

static void ek_next(void)
{
    uint32_t prev = ek_seq[ek_len];
    uint32_t best = 0xFFFFFFFFu;
    uint32_t x = prev;
    while (x > 1) {
        uint32_t p = ek_spf[x];
        while (x % p == 0) x /= p;
        uint32_t c = ek_ptr[p] ? ek_ptr[p] : p;
        for (;;) {
            if (c >= ek_vcap) {
                ek_grow_values(c);
                continue;
            }
            if (!ek_used[c]) break;
            c += p;
        }
        ek_ptr[p] = c;
        if (c < best) best = c;
    }
    ek_used[best] = 1;
    ek_push(best);
}

static uint32_t ekg_term(uint64_t m)
{
    if (ek_len == 0) {
        ek_grow_values(4096);
        ek_push(1);
        ek_push(2);
        ek_used[1] = ek_used[2] = 1;
    }
    while (ek_len < m) ek_next();
    return ek_seq[m];
}

static uint64_t *cyc = NULL;
static size_t cyc_len = 0, cyc_cap = 0;

seq_t a(idx_t n)
{
    if (cyc_len == 0) {
        cyc_cap = 64;
        cyc = (uint64_t *)malloc(cyc_cap * sizeof(uint64_t));
        if (!cyc) exit(1);
        cyc[0] = 73;
        cyc_len = 1;
    }
    while (cyc_len < (size_t)n) {
        if (cyc_len == cyc_cap) {
            cyc_cap *= 2;
            cyc = (uint64_t *)realloc(cyc, cyc_cap * sizeof(uint64_t));
            if (!cyc) exit(1);
        }
        cyc[cyc_len] = ekg_term(cyc[cyc_len - 1]);
        cyc_len++;
    }
    return (seq_t)cyc[n - 1];
}
