/*
 * A050023  Numbers k such that A050020(k) > A050020(k+1).
 * https://oeis.org/A050023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A050020(k) > A050020(k+1); A050020(1)=1,
 *            A050020(m)=floor(A050020(m-1)/2) if positive and unseen, else
 *            10*A050020(m-1). Computed by generation with a sorted seen-set
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *s_sorted = NULL;   /* sorted set of generated terms */
static idx_t s_cnt = 0, s_cap = 0;
static seq_t s_last = 1;
static idx_t *s_desc = NULL;     /* found descent positions */
static idx_t s_dn = 0, s_dcap = 0;

/* lower bound: first position with sorted[pos] >= v; sets found */
static idx_t A050023_lb(seq_t v, bool *found)
{
    idx_t lo = 0, hi = s_cnt;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (s_sorted[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    *found = (lo < s_cnt && s_sorted[lo] == v);
    return lo;
}

static void A050023_insert(seq_t v)
{
    bool f;
    idx_t pos = A050023_lb(v, &f);
    if (s_cnt >= s_cap) {
        idx_t nc = s_cap ? s_cap * 2 : 64;
        seq_t *p = mem_resize(seq_t, s_sorted, nc);
        if (!p) abort();
        s_sorted = p;
        s_cap = nc;
    }
    for (idx_t i = s_cnt; i > pos; --i)
        s_sorted[i] = s_sorted[i - 1];
    s_sorted[pos] = v;
    ++s_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (s_cnt == 0) {
        s_sorted = mem_new(seq_t, 64);
        if (!s_sorted) abort();
        s_cap = 64;
        s_last = 1;
        A050023_insert(s_last);
    }
    while (s_dn < n) {
        seq_t prev = s_last;
        seq_t cand = prev / 2;
        bool f = false;
        if (cand > 0) A050023_lb(cand, &f);
        seq_t nxt;
        if (cand > 0 && !f) nxt = cand;
        else nxt = prev * (seq_t)10;
        if (prev > nxt) {
            if (s_dn >= s_dcap) {
                idx_t nc = s_dcap ? s_dcap * 2 : 64;
                idx_t *p = (idx_t *)realloc(s_desc, (size_t)nc * sizeof(idx_t));
                if (!p) abort();
                s_desc = p;
                s_dcap = nc;
            }
            s_desc[s_dn++] = s_cnt;   /* prev is term number s_cnt */
        }
        A050023_insert(nxt);
        s_last = nxt;
    }
    return (seq_t)s_desc[n - 1];
}
