/*
 * A050086  Numbers k such that A050084(k) < A050084(k+1).
 * https://oeis.org/A050086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050084(k) < A050084(k+1); A050084:
 *            b(1)=1, b(n)=floor(b(n-1)/3) if that is nonzero and not among
 *            b(1..n-2), else 5*b(n-1). Computed by direct simulation with a
 *            sorted set
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A050086_set = NULL;
static idx_t A050086_len = 0, A050086_cap = 0;
static idx_t *A050086_desc = NULL;
static idx_t A050086_dcnt = 0, A050086_dcap = 0;
static idx_t A050086_m = 0;
static seq_t A050086_cur;

static idx_t A050086_lower(seq_t v)
{
    idx_t lo = 0, hi = A050086_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (A050086_set[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static void A050086_insert(seq_t v)
{
    if (A050086_len + 1 > A050086_cap) {
        idx_t nc = A050086_cap ? A050086_cap * 2 : 1024;
        seq_t *t = mem_resize(seq_t, A050086_set, nc);
        if (!t) abort();
        A050086_set = t;
        A050086_cap = nc;
    }
    idx_t pos = A050086_lower(v);
    for (idx_t j = A050086_len; j > pos; --j)
        A050086_set[j] = A050086_set[j - 1];
    A050086_set[pos] = v;
    A050086_len++;
}

static void A050086_push(idx_t k)
{
    if (A050086_dcnt + 1 > A050086_dcap) {
        idx_t nc = A050086_dcap ? A050086_dcap * 2 : 1024;
        idx_t *t = (idx_t *)realloc(A050086_desc, (size_t)nc * sizeof(idx_t));
        if (!t) abort();
        A050086_desc = t;
        A050086_dcap = nc;
    }
    A050086_desc[A050086_dcnt++] = k;
}

seq_t a(idx_t n)
{
    if (A050086_m == 0) {
        A050086_cur = (seq_t)1;
        A050086_m = 1;
        A050086_insert(A050086_cur);
    }
    while (A050086_dcnt < n) {
        seq_t cand = A050086_cur / 3;
        bool dup = (cand == (seq_t)0);
        if (!dup) {
            idx_t p = A050086_lower(cand);
            if (p < A050086_len && A050086_set[p] == cand) dup = true;
        }
        seq_t nxt = dup ? (seq_t)(A050086_cur * (seq_t)5) : cand;
        if (A050086_cur < nxt) A050086_push(A050086_m);
        A050086_cur = nxt;
        A050086_m++;
        A050086_insert(nxt);
    }
    return (seq_t)A050086_desc[n - 1];
}
