/*
 * A050021  a(n) = position of n in A050020.
 * https://oeis.org/A050021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of n in A050020, where A050020(1) = 1 and
 *            A050020(k) = floor(A050020(k-1)/2) if that is positive and not
 *            yet in the sequence, else 10*A050020(k-1); computed by
 *            generating A050020 with a hash index until n appears
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 967 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A50021_cache = NULL;
static idx_t A50021_size = 0, A50021_cap = 0;
static idx_t *A50021_tab = NULL;      /* stores index+1, 0 = empty */
static idx_t A50021_tcap = 0;

static idx_t A50021_hash(seq_t v)
{
    unsigned long long h = (unsigned long long)(v % (seq_t)1000003);
    h *= 11400714819323198485ULL;
    h ^= h >> 29;
    return (idx_t)h;
}

static idx_t A50021_find(seq_t v)   /* returns index+1 or 0 */
{
    idx_t mask = A50021_tcap - 1;
    idx_t p = A50021_hash(v) & mask;
    while (A50021_tab[p]) {
        if (A50021_cache[A50021_tab[p] - 1] == v)
            return A50021_tab[p];
        p = (p + 1) & mask;
    }
    return 0;
}

static void A50021_tab_insert(idx_t i)  /* i is 0-based cache index */
{
    idx_t mask = A50021_tcap - 1;
    idx_t p = A50021_hash(A50021_cache[i]) & mask;
    while (A50021_tab[p])
        p = (p + 1) & mask;
    A50021_tab[p] = i + 1;
}

static void A50021_push(seq_t v)
{
    if (A50021_size >= A50021_cap) {
        idx_t nc = A50021_cap ? A50021_cap * 2 : 64;
        seq_t *np = mem_resize(seq_t, A50021_cache, nc);
        if (!np) abort();
        A50021_cache = np;
        A50021_cap = nc;
    }
    A50021_cache[A50021_size] = v;
    if ((A50021_size + 1) * 2 > A50021_tcap) {
        idx_t nt = A50021_tcap ? A50021_tcap * 2 : 128;
        free(A50021_tab);
        A50021_tab = (idx_t *)calloc((size_t)nt, sizeof(idx_t));
        if (!A50021_tab) abort();
        A50021_tcap = nt;
        for (idx_t i = 0; i < A50021_size; ++i)
            A50021_tab_insert(i);
    }
    A50021_tab_insert(A50021_size);
    ++A50021_size;
}

seq_t a(idx_t n)
{
    if (A50021_size == 0)
        A50021_push((seq_t)1);

    seq_t target = (seq_t)n;
    for (;;) {
        idx_t f = A50021_find(target);
        if (f)
            return (seq_t)f;
        seq_t prev = A50021_cache[A50021_size - 1];
        seq_t cand = prev / 2;
        if (cand > 0 && !A50021_find(cand))
            A50021_push(cand);
        else
            A50021_push(prev * (seq_t)10);
    }
}
