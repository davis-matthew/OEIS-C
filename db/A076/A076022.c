/*
 * A076022  Position of 2n in A073662.
 * https://oeis.org/A076022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A073662(1)=2; A073662(m) = smallest unused even number v with
 *            A073662(m-1)*v+1 prime; a(n) = index m with A073662(m) = 2n
 *            (computed by direct generation/search)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *g_terms = NULL;
static size_t g_nt = 0, g_tcap = 0;
static size_t *g_pos = NULL;
static unsigned char *g_used = NULL;
static size_t g_ucap = 0;
static size_t g_low = 1;

static void ensure_k(size_t k) {
    if (k < g_ucap) return;
    size_t nc = g_ucap ? g_ucap : 64;
    while (nc <= k) nc *= 2;
    g_used = (unsigned char *)realloc(g_used, nc);
    g_pos = (size_t *)realloc(g_pos, nc * sizeof(size_t));
    memset(g_used + g_ucap, 0, nc - g_ucap);
    memset(g_pos + g_ucap, 0, (nc - g_ucap) * sizeof(size_t));
    g_ucap = nc;
}

static void push_term(size_t k) {
    if (g_nt == g_tcap) {
        g_tcap = g_tcap ? g_tcap * 2 : 64;
        g_terms = (uint64_t *)realloc(g_terms, g_tcap * sizeof(uint64_t));
    }
    ensure_k(k);
    g_terms[g_nt++] = 2 * (uint64_t)k;
    g_used[k] = 1;
    g_pos[k] = g_nt;
    while (g_low < g_ucap && g_used[g_low]) g_low++;
}

static void gen_next(void) {
    if (g_nt == 0) { push_term(1); return; }
    uint64_t prev = g_terms[g_nt - 1];
    size_t k = g_low;
    for (;; k++) {
        ensure_k(k);
        if (g_used[k]) continue;
        if (is_prime_u64(prev * 2 * (uint64_t)k + 1)) break;
    }
    push_term(k);
}

seq_t a(idx_t n) {
    size_t k = (size_t)n;
    ensure_k(k);
    while (g_pos[k] == 0) {
        gen_next();
        if (k >= g_ucap) ensure_k(k);
    }
    return (seq_t)g_pos[k];
}
