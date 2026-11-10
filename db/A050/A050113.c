/*
 * A050113  a(n) = position of n in A050112.
 * https://oeis.org/A050113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050112, where A050112(1) = 1 and
 *            A050112(k) = floor(A050112(k-1)/4) if positive and not yet used,
 *            else 6*A050112(k-1); computed by generating A050112 with a hash
 *            set until n appears
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050113_seq = NULL;
static idx_t A050113_cnt = 0, A050113_cap = 0;
static idx_t *A050113_tab = NULL;
static idx_t A050113_tcap = 0;

static idx_t A050113_hash(seq_t x, idx_t mask)
{
    seq_t h = x ^ (x >> 13) ^ (x >> 29) ^ (x >> 47);
    h = h & (seq_t)mask;
    return (idx_t)(unsigned long long)h;
}

static idx_t A050113_find(seq_t x)
{
    idx_t mask = A050113_tcap - 1;
    idx_t h = A050113_hash(x, mask);
    while (A050113_tab[h] != 0) {
        if (A050113_seq[A050113_tab[h] - 1] == x) return A050113_tab[h];
        h = (h + 1) & mask;
    }
    return 0;
}

static void A050113_insert(idx_t idx)
{
    idx_t mask = A050113_tcap - 1;
    idx_t h = A050113_hash(A050113_seq[idx], mask);
    while (A050113_tab[h] != 0) h = (h + 1) & mask;
    A050113_tab[h] = idx + 1;
}

static void A050113_push(seq_t v)
{
    if (A050113_cnt == A050113_cap) {
        idx_t nc = A050113_cap ? A050113_cap * 2 : 1024;
        seq_t *nb = mem_resize(seq_t, A050113_seq, nc);
        if (!nb) abort();
        A050113_seq = nb;
        A050113_cap = nc;
    }
    if ((A050113_cnt + 1) * 2 > A050113_tcap) {
        idx_t nt = A050113_tcap ? A050113_tcap * 2 : 2048;
        idx_t *t = mem_new(idx_t, nt);
        if (!t) abort();
        if (A050113_tab) mem_free(A050113_tab);
        A050113_tab = t;
        A050113_tcap = nt;
        for (idx_t i = 0; i < A050113_cnt; ++i) A050113_insert(i);
    }
    A050113_seq[A050113_cnt] = v;
    A050113_insert(A050113_cnt);
    A050113_cnt++;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A050113_cnt == 0) A050113_push((seq_t)1);
    idx_t p = A050113_find((seq_t)n);
    while (p == 0) {
        seq_t prev = A050113_seq[A050113_cnt - 1];
        seq_t cand = prev / 4;
        seq_t next;
        if (cand > 0 && A050113_find(cand) == 0) next = cand;
        else next = prev * 6;
        A050113_push(next);
        if (next == (seq_t)n) { p = A050113_cnt; break; }
    }
    return (seq_t)p;
}
