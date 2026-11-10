/*
 * A064665  Regard A064413 as giving a permutation of the positive integers;
 *          sequence gives first infinite cycle, beginning at its smallest
 *          term, 7.
 * https://oeis.org/A064665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7, a(n+1) = E(a(n)) where E = A064413 (EKG sequence:
 *            E(1)=1, E(2)=2, E(n) = smallest unused number sharing a factor
 *            with E(n-1)); computed by iterating the permutation with an
 *            incremental EKG generator
 * Range:     u8 n <= 29, u16 n <= 177, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Validated: standard, pass, 71 terms through n = 399 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A064665_ekg = NULL;   /* 1-based EKG terms */
static size_t A064665_len = 0, A064665_ecap = 0;
static unsigned char *A064665_used = NULL;
static uint64_t *A064665_ptr = NULL;
static size_t A064665_vcap = 0;
static uint64_t *A064665_cyc = NULL;
static size_t A064665_cn = 0, A064665_ccap = 0;

static void A064665_grow_vals(size_t need)
{
    if (need < A064665_vcap) return;
    size_t nc = A064665_vcap ? A064665_vcap : 1024;
    while (nc <= need) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(A064665_used, nc);
    uint64_t *p = (uint64_t *)realloc(A064665_ptr, nc * sizeof(uint64_t));
    if (!u || !p) exit(1);
    memset(u + A064665_vcap, 0, nc - A064665_vcap);
    memset(p + A064665_vcap, 0, (nc - A064665_vcap) * sizeof(uint64_t));
    A064665_used = u;
    A064665_ptr = p;
    A064665_vcap = nc;
}

static void A064665_push(uint64_t v)
{
    if (A064665_len + 1 >= A064665_ecap) {
        size_t nc = A064665_ecap ? A064665_ecap * 2 : 1024;
        uint64_t *e = (uint64_t *)realloc(A064665_ekg, nc * sizeof(uint64_t));
        if (!e) exit(1);
        A064665_ekg = e;
        A064665_ecap = nc;
    }
    A064665_ekg[++A064665_len] = v;
}

static void A064665_ekg_next(void)
{
    if (A064665_len == 0) {
        A064665_grow_vals(1024);
        A064665_push(1);
        A064665_push(2);
        A064665_used[1] = 1;
        A064665_used[2] = 1;
        A064665_ptr[2] = 4;
        return;
    }
    uint64_t prev = A064665_ekg[A064665_len];
    uint64_t x = prev, best = 0;
    for (uint64_t p = 2; x > 1; p += (p == 2 ? 1 : 2)) {
        if (p * p > x) p = x;
        if (x % p != 0) continue;
        while (x % p == 0) x /= p;
        uint64_t c = A064665_ptr[p] ? A064665_ptr[p] : p;
        for (;;) {
            if (c >= A064665_vcap) A064665_grow_vals((size_t)c);
            if (!A064665_used[c]) break;
            c += p;
        }
        A064665_ptr[p] = c;
        if (best == 0 || c < best) best = c;
    }
    A064665_grow_vals((size_t)best);
    A064665_used[best] = 1;
    A064665_push(best);
}

seq_t a(idx_t n)
{
    if (A064665_cn == 0) {
        A064665_ccap = 64;
        A064665_cyc = (uint64_t *)malloc(A064665_ccap * sizeof(uint64_t));
        if (!A064665_cyc) exit(1);
        A064665_cyc[0] = 7;
        A064665_cn = 1;
    }
    while ((idx_t)A064665_cn < n) {
        uint64_t x = A064665_cyc[A064665_cn - 1];
        while (A064665_len < x) A064665_ekg_next();
        if (A064665_cn >= A064665_ccap) {
            A064665_ccap *= 2;
            A064665_cyc = (uint64_t *)realloc(A064665_cyc, A064665_ccap * sizeof(uint64_t));
            if (!A064665_cyc) exit(1);
        }
        A064665_cyc[A064665_cn++] = A064665_ekg[x];
    }
    return (seq_t)A064665_cyc[n - 1];
}
