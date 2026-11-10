/*
 * A064004  At step n in computing A064413, sequence gives smallest multiple
 *          of 2 not yet seen, divided by 2.
 * https://oeis.org/A064004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m/2 where m is the smallest even number not among the
 *            first n terms of the EKG sequence A064413 (A064413(1)=1,
 *            A064413(2)=2, then smallest unused number sharing a factor with
 *            the previous term); computed by direct simulation
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *A064004_used = NULL;
static uint64_t *A064004_nxt = NULL;
static size_t A064004_cap = 0;
static uint64_t *A064004_res = NULL;
static size_t A064004_rescap = 0;
static size_t A064004_t = 0;
static uint64_t A064004_last = 0;
static uint64_t A064004_e = 2;

static void A064004_ensure(size_t v)
{
    if (v < A064004_cap) return;
    size_t nc = A064004_cap ? A064004_cap : 1024;
    while (nc <= v) nc *= 2;
    unsigned char *u = (unsigned char *)realloc(A064004_used, nc);
    uint64_t *x = (uint64_t *)realloc(A064004_nxt, nc * sizeof(uint64_t));
    if (!u || !x) exit(1);
    memset(u + A064004_cap, 0, nc - A064004_cap);
    memset(x + A064004_cap, 0, (nc - A064004_cap) * sizeof(uint64_t));
    A064004_used = u;
    A064004_nxt = x;
    A064004_cap = nc;
}

static void A064004_step(void)
{
    uint64_t term;
    if (A064004_t == 0) term = 1;
    else if (A064004_t == 1) term = 2;
    else {
        uint64_t x = A064004_last, best = 0;
        for (uint64_t p = 2; p * p <= x || x > 1; p++) {
            if (p * p > x) p = x;
            if (x % p) continue;
            while (x % p == 0) x /= p;
            A064004_ensure((size_t)p);
            uint64_t c = A064004_nxt[p];
            if (c == 0) c = p;
            for (;;) {
                A064004_ensure((size_t)c);
                if (!A064004_used[c]) break;
                c += p;
            }
            A064004_nxt[p] = c;
            if (best == 0 || c < best) best = c;
            if (x == 1) break;
        }
        term = best;
    }
    A064004_ensure((size_t)term);
    A064004_used[term] = 1;
    A064004_last = term;
    A064004_t++;
    for (;;) {
        A064004_ensure((size_t)A064004_e);
        if (!A064004_used[A064004_e]) break;
        A064004_e += 2;
    }
    if (A064004_t >= A064004_rescap) {
        size_t nc = A064004_rescap ? A064004_rescap * 2 : 1024;
        uint64_t *r = (uint64_t *)realloc(A064004_res, nc * sizeof(uint64_t));
        if (!r) exit(1);
        A064004_res = r;
        A064004_rescap = nc;
    }
    A064004_res[A064004_t] = A064004_e / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while ((idx_t)A064004_t < n) A064004_step();
    return (seq_t)A064004_res[n];
}
