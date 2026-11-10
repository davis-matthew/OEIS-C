/*
 * A091413  Partial sums of A090822.
 * https://oeis.org/A091413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A090822(k), a(0) = 0; A090822 (Gijswijt's
 *            sequence) computed by direct search
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9230 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *G_cache = NULL;
static idx_t G_len = 0;
static idx_t G_cap = 0;

static void G_ensure(idx_t needed)
{
    if (G_cap >= needed) return;
    idx_t new_cap = G_cap ? G_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    G_cache = mem_resize(seq_t, G_cache, new_cap);
    G_cap = new_cap;
}

static void G_next(void)
{
    idx_t len = G_len;
    G_ensure(len + 1);
    seq_t best = 0;
    for (idx_t block_len = 1; block_len <= len; ++block_len) {
        idx_t start = len - block_len;
        idx_t count = 1;
        while (start >= block_len) {
            bool match = true;
            for (idx_t j = 0; j < block_len; ++j) {
                if (G_cache[start + j] != G_cache[start - block_len + j]) {
                    match = false;
                    break;
                }
            }
            if (!match) break;
            ++count;
            start -= block_len;
        }
        if ((seq_t)count > best) best = (seq_t)count;
    }
    G_cache[G_len] = best;
    ++G_len;
}

static seq_t A090822_a(idx_t n)
{
    if (G_len < 1) {
        G_ensure(2);
        G_cache[0] = (seq_t)1;
        G_cache[1] = (seq_t)1;
        G_len = 2;
    }
    while (G_len < n) G_next();
    return G_cache[n - 1];
}

static seq_t *P_cache = NULL;   /* P_cache[i] = a(i) */
static idx_t P_len = 0;
static idx_t P_cap = 0;

seq_t a(idx_t n)
{
    if (P_len == 0) {
        P_cap = 16;
        P_cache = mem_new(seq_t, P_cap);
        P_cache[0] = (seq_t)0;
        P_len = 1;
    }
    if (P_cap < n + 1) {
        idx_t nc = P_cap;
        while (nc < n + 1) nc <<= 1;
        P_cache = mem_resize(seq_t, P_cache, nc);
        P_cap = nc;
    }
    while (P_len <= n) {
        P_cache[P_len] = P_cache[P_len - 1] + A090822_a(P_len);
        ++P_len;
    }
    return P_cache[n];
}
