/*
 * A050109  a(n)=position of n in A050108.
 * https://oeis.org/A050109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050108, where A050108(1)=1 and
 *            A050108(k) = floor(A050108(k-1)/4) if that is positive and not
 *            yet seen, else 5*A050108(k-1); computed by simulating A050108
 *            with a hash index until n appears
 * Range:     u8 n <= 8, u16 n <= 33, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A050109_M 1048573

static seq_t *A050109_terms = NULL;
static size_t *A050109_nxt = NULL;
static size_t *A050109_bucket = NULL;
static size_t A050109_cap = 0;
static size_t A050109_cnt = 0;
static bool A050109_over = false;

static size_t A050109_hash(seq_t v)
{
    return (size_t)(long long)(v % (seq_t)A050109_M);
}

/* returns 1-based position of v, or 0 */
static size_t A050109_find(seq_t v)
{
    size_t p = A050109_bucket[A050109_hash(v)];
    while (p != 0) {
        if (A050109_terms[p - 1] == v) return p;
        p = A050109_nxt[p - 1];
    }
    return 0;
}

static void A050109_push(seq_t v)
{
    if (A050109_cnt >= A050109_cap) {
        size_t ns = A050109_cap == 0 ? 256 : A050109_cap * 2;
        A050109_terms = mem_resize(seq_t, A050109_terms, ns);
        A050109_nxt = mem_resize(size_t, A050109_nxt, ns);
        if (!A050109_terms || !A050109_nxt) abort();
        A050109_cap = ns;
    }
    size_t h = A050109_hash(v);
    A050109_terms[A050109_cnt] = v;
    A050109_nxt[A050109_cnt] = A050109_bucket[h];
    A050109_cnt++;
    A050109_bucket[h] = A050109_cnt;
}

static void A050109_step(void)
{
    if (A050109_bucket == NULL) {
        A050109_bucket = mem_new(size_t, A050109_M);
        if (!A050109_bucket) abort();
    }
    if (A050109_cnt == 0) {
        A050109_push((seq_t)1);
        return;
    }
    seq_t prev = A050109_terms[A050109_cnt - 1];
    seq_t cand = prev / 4;
    if (cand > 0 && A050109_find(cand) == 0) {
        A050109_push(cand);
        return;
    }
    seq_t nx = (seq_t)(prev * 5);
    if (nx / 5 != prev || nx < prev) {
        A050109_over = true;
        return;
    }
    A050109_push(nx);
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    for (;;) {
        if (A050109_bucket != NULL) {
            size_t p = A050109_find(target);
            if (p != 0) return (seq_t)p;
        }
        if (A050109_over) return (seq_t)0;
        A050109_step();
    }
}
