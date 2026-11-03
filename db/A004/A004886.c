/*
 * A004886  Numbers that are the sum of at most 2 positive 9th powers.
 * https://oeis.org/A004886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   ordered merge search: a(n) = n-th smallest distinct value of i^9
 *            + j^9 with 0 <= i <= j (0 counted as 0^9 + 0^9), a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 77, u64 n <= 9545,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 8799 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *P = NULL;      /* P[i] = i^9 */
static bool *Pinf = NULL;    /* true if i^9 overflows seq_t */
static idx_t Plen = 0, Pcap = 0;

static seq_t *Memo = NULL;
static idx_t Mlen = 0, Mcap = 0;

static idx_t *J = NULL;      /* J[r] = current second index for row r */
static idx_t nrows = 0, Jcap = 0;
static bool exhausted = false;

static void ensure_pow(idx_t k)
{
    if (k < Plen) return;
    if (k + 1 > Pcap) {
        idx_t nc = Pcap ? Pcap : 64;
        while (nc < k + 1) nc *= 2;
        if (P == NULL) P = alloc_zero(nc);
        else P = grow_zero(P, Pcap, nc);
        Pinf = (bool *)realloc(Pinf, nc * sizeof(bool));
        Pcap = nc;
    }
    while (Plen <= k) {
        idx_t i = Plen;
        seq_t b = (seq_t)i;
        seq_t v = 1;
        bool inf = false;
        int t;
        for (t = 0; t < 9 && !inf; t++) {
            seq_t nv = v * b;
            if (i != 0 && nv / b != v) inf = true;
            else v = nv;
        }
        P[i] = inf ? (seq_t)0 : v;
        Pinf[i] = inf;
        Plen++;
    }
}

/* value of i^9 + j^9; returns false if it overflows seq_t */
static bool pair_val(idx_t i, idx_t j, seq_t *out)
{
    seq_t s;
    ensure_pow(i > j ? i : j);
    if (Pinf[i] || Pinf[j]) return false;
    s = P[i] + P[j];
    if (s < P[i]) return false;
    *out = s;
    return true;
}

static void push_memo(seq_t v)
{
    if (Mlen + 1 > Mcap) {
        idx_t nc = Mcap ? Mcap * 2 : 256;
        if (Memo == NULL) Memo = alloc_zero(nc);
        else Memo = grow_zero(Memo, Mcap, nc);
        Mcap = nc;
    }
    Memo[Mlen++] = v;
}

static void ensure_rows(idx_t k)
{
    if (k + 1 > Jcap) {
        idx_t nc = Jcap ? Jcap : 64;
        while (nc < k + 1) nc *= 2;
        J = (idx_t *)realloc(J, nc * sizeof(idx_t));
        Jcap = nc;
    }
}

static void step(void)
{
    bool have = false;
    seq_t best = 0, v = 0;
    idx_t r;
    bool newv_ok;
    seq_t newv = 0;

    for (r = 0; r < nrows; r++) {
        if (pair_val(r, J[r], &v)) {
            if (!have || v < best) { best = v; have = true; }
        }
    }
    newv_ok = pair_val(nrows, nrows, &newv);
    if (newv_ok && (!have || newv < best)) { best = newv; have = true; }

    if (!have) { exhausted = true; return; }

    for (r = 0; r < nrows; r++) {
        if (pair_val(r, J[r], &v) && v == best) J[r]++;
    }
    if (newv_ok && newv == best) {
        ensure_rows(nrows);
        J[nrows] = nrows + 1;
        nrows++;
    }
    if (Mlen == 0 || Memo[Mlen - 1] != best) push_memo(best);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (Mlen < n && !exhausted) step();
    if (Mlen < n) return 0;
    return Memo[n - 1];
}
