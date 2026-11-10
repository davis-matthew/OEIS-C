/*
 * A050107  Numbers k such that A050104(k) > A050104(k+1).
 * https://oeis.org/A050107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050104(k) > A050104(k+1), where
 *            A050104(1) = 1, A050104(m) = floor(A050104(m-1)/4) if that is
 *            nonzero and not among earlier terms, else 3*A050104(m-1);
 *            computed by direct simulation with a hash set
 * Range:     u8 n <= 19, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050107_vals = 0;
static size_t A050107_len = 0, A050107_cap = 0;
static seq_t *A050107_tab = 0;
static unsigned char *A050107_occ = 0;
static size_t A050107_tcap = 0;
static idx_t *A050107_desc = 0;
static size_t A050107_dcnt = 0, A050107_dcap = 0;

static size_t A050107_hash(seq_t x, size_t mask)
{
    size_t lo = (size_t)(long long)(x & (seq_t)0xFFFFFFFFu);
    return (lo * (size_t)2654435761u) & mask;
}

static void A050107_insert_raw(seq_t x)
{
    size_t mask = A050107_tcap - 1;
    size_t h = A050107_hash(x, mask);
    while (A050107_occ[h]) {
        if (A050107_tab[h] == x) return;
        h = (h + 1) & mask;
    }
    A050107_occ[h] = 1;
    A050107_tab[h] = x;
}

static bool A050107_contains(seq_t x)
{
    size_t mask = A050107_tcap - 1;
    size_t h = A050107_hash(x, mask);
    while (A050107_occ[h]) {
        if (A050107_tab[h] == x) return true;
        h = (h + 1) & mask;
    }
    return false;
}

static void A050107_insert(seq_t x)
{
    if ((A050107_len + 1) * 2 > A050107_tcap) {
        size_t ncap = A050107_tcap ? A050107_tcap * 2 : 1024;
        seq_t *ot = A050107_tab;
        unsigned char *oo = A050107_occ;
        size_t oc = A050107_tcap;
        A050107_tab = mem_new(seq_t, ncap);
        A050107_occ = mem_new(unsigned char, ncap);
        A050107_tcap = ncap;
        for (size_t i = 0; i < oc; i++)
            if (oo[i]) A050107_insert_raw(ot[i]);
        if (ot) mem_free(ot);
        if (oo) mem_free(oo);
    }
    A050107_insert_raw(x);
}

static void A050107_push(seq_t x)
{
    if (A050107_len == A050107_cap) {
        size_t nc = A050107_cap ? A050107_cap * 2 : 1024;
        if (A050107_vals) A050107_vals = mem_resize(seq_t, A050107_vals, nc);
        else A050107_vals = mem_new(seq_t, nc);
        A050107_cap = nc;
    }
    A050107_insert(x);
    A050107_vals[A050107_len++] = x;
}

static void A050107_push_desc(idx_t k)
{
    if (A050107_dcnt == A050107_dcap) {
        size_t nc = A050107_dcap ? A050107_dcap * 2 : 256;
        idx_t *nd = mem_new(idx_t, nc);
        for (size_t i = 0; i < A050107_dcnt; i++) nd[i] = A050107_desc[i];
        if (A050107_desc) mem_free(A050107_desc);
        A050107_desc = nd;
        A050107_dcap = nc;
    }
    A050107_desc[A050107_dcnt++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A050107_len == 0) A050107_push((seq_t)1);
    while ((idx_t)A050107_dcnt < n) {
        seq_t prev = A050107_vals[A050107_len - 1];
        seq_t cand = prev / 4;
        bool used = (cand == 0) || A050107_contains(cand);
        seq_t next = used ? (seq_t)(prev * 3) : cand;
        idx_t k = (idx_t)A050107_len; /* 1-based index of prev */
        A050107_push(next);
        if (prev > next) A050107_push_desc(k);
    }
    return (seq_t)A050107_desc[n - 1];
}
