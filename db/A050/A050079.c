/*
 * A050079  Numbers k such that A050076(k) > A050076(k+1).
 * https://oeis.org/A050079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A050076(k) > A050076(k+1); A050076:
 *            b(1)=1, b(m)=floor(b(m-1)/3) if that is nonzero and unused, else
 *            2*b(m-1). Computed by direct generation with a sorted set
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A050079_terms = NULL;   /* sequence terms in order */
static seq_t *A050079_sorted = NULL;  /* sorted set of used values */
static size_t A050079_cnt = 0, A050079_cap = 0;
static size_t *A050079_desc = NULL;
static size_t A050079_ndesc = 0, A050079_dcap = 0;

/* binary search: returns position of first element >= v; sets found */
static size_t A050079_lower(seq_t v, bool *found)
{
    size_t lo = 0, hi = A050079_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A050079_sorted[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    *found = (lo < A050079_cnt && A050079_sorted[lo] == v);
    return lo;
}

static void A050079_reserve(void)
{
    if (A050079_cnt + 1 <= A050079_cap) return;
    size_t nc = A050079_cap ? A050079_cap * 2 : 64;
    seq_t *t = mem_resize(seq_t, A050079_terms, nc);
    if (!t) exit(EXIT_FAILURE);
    A050079_terms = t;
    seq_t *s = mem_resize(seq_t, A050079_sorted, nc);
    if (!s) exit(EXIT_FAILURE);
    A050079_sorted = s;
    A050079_cap = nc;
}

static void A050079_push(seq_t v)
{
    A050079_reserve();
    bool f;
    size_t pos = A050079_lower(v, &f);
    if (!f) {
        for (size_t i = A050079_cnt; i > pos; --i)
            A050079_sorted[i] = A050079_sorted[i - 1];
        A050079_sorted[pos] = v;
    }
    A050079_terms[A050079_cnt] = v;
    ++A050079_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t want = (size_t)n;
    if (A050079_cnt == 0) A050079_push((seq_t)1);
    while (A050079_ndesc < want) {
        seq_t prev = A050079_terms[A050079_cnt - 1];
        seq_t cand = prev / (seq_t)3;
        bool used = true;
        if (cand != (seq_t)0) {
            bool f;
            A050079_lower(cand, &f);
            used = f;
        }
        seq_t next = used ? prev * (seq_t)2 : cand;
        if (prev > next) {
            if (A050079_ndesc >= A050079_dcap) {
                size_t nc = A050079_dcap ? A050079_dcap * 2 : 64;
                size_t *d = (size_t *)realloc(A050079_desc, nc * sizeof(size_t));
                if (!d) exit(EXIT_FAILURE);
                A050079_desc = d;
                A050079_dcap = nc;
            }
            A050079_desc[A050079_ndesc++] = A050079_cnt; /* index of prev (1-based) */
        }
        A050079_push(next);
    }
    return (seq_t)A050079_desc[want - 1];
}
