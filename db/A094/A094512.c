/*
 * A094512  Inverse of A094511.
 * https://oeis.org/A094512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A094511(k) = n (inverse permutation);
 *            computed by running the A094511 greedy construction forward
 *            until n appears
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8787 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static size_t *A094512_par = NULL;
static size_t A094512_cap = 0;
static size_t *A094512_inv = NULL;
static size_t A094512_invcap = 0;
static size_t A094512_next = 1;

static void A094512_grow(size_t x)
{
    if (x < A094512_cap) return;
    size_t nc = A094512_cap * 2;
    if (nc < x + 2) nc = x + 2;
    if (nc < 1024) nc = 1024;
    size_t *t = (size_t *)realloc(A094512_par, nc * sizeof(size_t));
    if (!t) exit(1);
    for (size_t i = A094512_cap; i < nc; ++i) t[i] = i;
    A094512_par = t;
    A094512_cap = nc;
}

static void A094512_growinv(size_t x)
{
    if (x < A094512_invcap) return;
    size_t nc = A094512_invcap * 2;
    if (nc < x + 2) nc = x + 2;
    if (nc < 1024) nc = 1024;
    size_t *t = (size_t *)realloc(A094512_inv, nc * sizeof(size_t));
    if (!t) exit(1);
    for (size_t i = A094512_invcap; i < nc; ++i) t[i] = 0;
    A094512_inv = t;
    A094512_invcap = nc;
}

static size_t A094512_find(size_t x)
{
    A094512_grow(x);
    size_t r = x;
    while (A094512_par[r] != r) r = A094512_par[r];
    while (A094512_par[x] != r) {
        size_t nx = A094512_par[x];
        A094512_par[x] = r;
        x = nx;
    }
    return r;
}

static unsigned A094512_pop(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
}

static void A094512_step(void)
{
    size_t i = A094512_next++;
    unsigned ones = A094512_pop((uint64_t)i);
    size_t cand = 1;
    for (;;) {
        cand = A094512_find(cand);
        if (cand == i || A094512_pop((uint64_t)cand) > ones) {
            cand++;
            continue;
        }
        A094512_grow(cand + 1);
        A094512_par[cand] = cand + 1;
        A094512_growinv(cand);
        A094512_inv[cand] = i;
        return;
    }
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    A094512_growinv(m);
    while (A094512_inv[m] == 0) A094512_step();
    return (seq_t)A094512_inv[m];
}
