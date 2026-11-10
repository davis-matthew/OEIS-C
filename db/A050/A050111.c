/*
 * A050111  Numbers k such that A050108(k) > A050108(k+1).
 * https://oeis.org/A050111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A050108(k) > A050108(k+1); A050108: b(1)=1,
 *            b(n)=floor(b(n-1)/4) if positive and not yet seen, else
 *            5*b(n-1). Computed by simulation with a hash set
 * Range:     u8 none, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *s_terms = NULL;
static size_t s_tcount = 0, s_tcap = 0;
static size_t *s_hash = NULL;
static size_t s_hcap = 0;
static idx_t *s_desc = NULL;
static size_t s_dcount = 0, s_dcap = 0;

static size_t s_slot(seq_t x, size_t cap)
{
    return (size_t)(x % (seq_t)cap);
}

static void s_insert_idx(size_t i)
{
    size_t h = s_slot(s_terms[i], s_hcap);
    while (s_hash[h] != 0) {
        h++;
        if (h == s_hcap) h = 0;
    }
    s_hash[h] = i + 1;
}

static void s_rehash(size_t newcap)
{
    free(s_hash);
    s_hcap = newcap;
    s_hash = (size_t *)calloc(s_hcap, sizeof(size_t));
    if (!s_hash) abort();
    for (size_t i = 0; i < s_tcount; ++i) s_insert_idx(i);
}

static bool s_contains(seq_t x)
{
    size_t h = s_slot(x, s_hcap);
    while (s_hash[h] != 0) {
        if (s_terms[s_hash[h] - 1] == x) return true;
        h++;
        if (h == s_hcap) h = 0;
    }
    return false;
}

static void s_step(void)
{
    if (s_tcount == 0) {
        s_tcap = 64;
        s_terms = mem_new(seq_t, s_tcap);
        s_terms[0] = 1;
        s_tcount = 1;
        s_rehash(256);
        return;
    }
    seq_t prev = s_terms[s_tcount - 1];
    seq_t cand = prev / 4;
    seq_t next;
    bool desc = false;
    if (cand > 0 && !s_contains(cand)) {
        next = cand;
        desc = true;
    } else {
        next = 5 * prev;
    }
    if (s_tcount == s_tcap) {
        size_t nc = s_tcap * 2;
        seq_t *t = mem_resize(seq_t, s_terms, nc);
        if (!t) abort();
        s_terms = t;
        s_tcap = nc;
    }
    if (desc) {
        if (s_dcount == s_dcap) {
            s_dcap = s_dcap ? s_dcap * 2 : 64;
            s_desc = (idx_t *)realloc(s_desc, s_dcap * sizeof(idx_t));
            if (!s_desc) abort();
        }
        s_desc[s_dcount++] = (idx_t)s_tcount; /* k = index of prev (1-based) */
    }
    s_terms[s_tcount] = next;
    s_tcount++;
    if (s_tcount * 2 > s_hcap) s_rehash(s_hcap * 2);
    else s_insert_idx(s_tcount - 1);
}

seq_t a(idx_t n)
{
    while (s_dcount < (size_t)n) s_step();
    return (seq_t)s_desc[n - 1];
}
