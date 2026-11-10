/*
 * A088337  Self-inverse integer permutation induced by A088230.
 * https://oeis.org/A088337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the 0-based position of the other occurrence of the value
 *            A088230(n+1) in A088230 (A088230(a(n)+1) = A088230(n+1), a(n) !=
 *            n); computed by simulating A088230 (a(1)=0; a(k)=a(k-1)-(k-1) if
 *            nonnegative and seen at most once, else a(k-1)+(k-1)) with a
 *            hash table of occurrences
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef struct {
    uint64_t key;
    uint64_t p1, p2;
    unsigned cnt;
} A088337_ent;

static A088337_ent *A088337_tab = NULL;
static size_t A088337_cap = 0, A088337_used = 0;
static seq_t *A088337_terms = NULL;
static size_t A088337_tcap = 0, A088337_size = 0;

static size_t A088337_slot(A088337_ent *tab, size_t cap, uint64_t key)
{
    size_t h = (size_t)((key * 0x9E3779B97F4A7C15ULL) >> 20) & (cap - 1);
    while (tab[h].cnt != 0 && tab[h].key != key) h = (h + 1) & (cap - 1);
    return h;
}

static void A088337_grow(void)
{
    size_t ncap = A088337_cap ? A088337_cap * 2 : 1024;
    A088337_ent *nt = (A088337_ent *)calloc(ncap, sizeof(A088337_ent));
    if (!nt) abort();
    for (size_t i = 0; i < A088337_cap; i++) {
        if (A088337_tab[i].cnt) {
            size_t s = A088337_slot(nt, ncap, A088337_tab[i].key);
            nt[s] = A088337_tab[i];
        }
    }
    free(A088337_tab);
    A088337_tab = nt;
    A088337_cap = ncap;
}

static unsigned A088337_count(uint64_t key)
{
    if (!A088337_cap) return 0;
    size_t s = A088337_slot(A088337_tab, A088337_cap, key);
    return A088337_tab[s].cnt;
}

static void A088337_add(uint64_t key, uint64_t pos)
{
    if ((A088337_used + 1) * 2 > A088337_cap) A088337_grow();
    size_t s = A088337_slot(A088337_tab, A088337_cap, key);
    if (A088337_tab[s].cnt == 0) {
        A088337_tab[s].key = key;
        A088337_tab[s].p1 = pos;
        A088337_tab[s].cnt = 1;
        A088337_used++;
    } else {
        if (A088337_tab[s].cnt == 1) A088337_tab[s].p2 = pos;
        A088337_tab[s].cnt++;
    }
}

static void A088337_extend(void)
{
    if (A088337_size == 0) {
        A088337_tcap = 1024;
        A088337_terms = mem_new(seq_t, A088337_tcap);
        A088337_terms[0] = 0;
        A088337_size = 1;
        A088337_add(0, 1);
        return;
    }
    if (A088337_size >= A088337_tcap) {
        size_t nc = A088337_tcap * 2;
        A088337_terms = mem_resize(seq_t, A088337_terms, nc);
        A088337_tcap = nc;
    }
    uint64_t i = (uint64_t)A088337_size + 1;
    seq_t step = (seq_t)(i - 1);
    seq_t prev = A088337_terms[A088337_size - 1];
    seq_t val = prev + step;
    if (prev >= step) {
        seq_t cand = prev - step;
        if (A088337_count((uint64_t)(unsigned long long)cand) <= 1) val = cand;
    }
    A088337_terms[A088337_size++] = val;
    A088337_add((uint64_t)(unsigned long long)val, i);
}

seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)n + 1;
    if (A088337_size == 0) A088337_extend();
    while (A088337_size < p) A088337_extend();
    uint64_t key = (uint64_t)(unsigned long long)A088337_terms[p - 1];
    while (A088337_count(key) < 2) A088337_extend();
    size_t s = A088337_slot(A088337_tab, A088337_cap, key);
    uint64_t other = (A088337_tab[s].p1 == p) ? A088337_tab[s].p2 : A088337_tab[s].p1;
    return (seq_t)(other - 1);
}
