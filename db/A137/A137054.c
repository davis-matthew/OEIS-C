/*
 * A137054  Numbers k such that k and k^2 use only the digits 1, 4, 6 and 8.
 * https://oeis.org/A137054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_counts, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search all numbers whose decimal digits belong to {1,4,6,8}
   and whose square also uses only these digits; numbers are emitted
   in increasing order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/seq_mul.h"
#include "../utils/digit_counts.h"

/* ---------- Helper to test allowed digits ---------- */
static bool only_allowed(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] && (d != 1 && d != 4 && d != 6 && d != 8)) {
            return false;
        }
    }
    return true;
}

/* ---------- DYNAMIC ARRAY FOR seq_t ---------- */
struct dyn_array {
    seq_t *data;
    size_t len;
    size_t cap;
};

static void dyn_init(struct dyn_array *a)
{
    a->data = NULL;
    a->len = 0;
    a->cap = 0;
}

static void dyn_grow(struct dyn_array *a, size_t need)
{
    if (a->cap >= need) return;
    size_t newcap = a->cap ? (a->cap << 1) : 16;
    if (newcap < need) newcap = need;
    seq_t *newdata = (seq_t *)malloc(sizeof(seq_t) * newcap);
    if (a->data) {
        for (size_t i = 0; i < a->len; ++i) {
            newdata[i] = a->data[i];
        }
        free(a->data);
    }
    a->data = newdata;
    a->cap = newcap;
}

static void dyn_push(struct dyn_array *a, seq_t v)
{
    dyn_grow(a, a->len + 1);
    a->data[a->len++] = v;
}

/* ---------- MIN-HEAP FOR seq_t ---------- */
struct heap {
    seq_t *data;
    size_t size;
    size_t cap;
};

static void heap_init(struct heap *h)
{
    h->data = NULL;
    h->size = 0;
    h->cap  = 0;
}

static void heap_grow(struct heap *h)
{
    size_t newcap = h->cap ? (h->cap << 1) : 16;
    seq_t *newdata = (seq_t *)malloc(sizeof(seq_t) * newcap);
    if (h->data) {
        for (size_t i = 0; i < h->size; ++i) {
            newdata[i] = h->data[i];
        }
        free(h->data);
    }
    h->data = newdata;
    h->cap  = newcap;
}

static void heap_push(struct heap *h, seq_t v)
{
    if (h->size == h->cap) heap_grow(h);
    size_t i = h->size++;
    h->data[i] = v;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= h->data[i]) break;
        seq_t t = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = t;
        i = p;
    }
}

static seq_t heap_pop(struct heap *h)
{
    seq_t ret = h->data[0];
    seq_t last = h->data[--h->size];
    if (h->size == 0) return ret;
    h->data[0] = last;
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t s = l;
        if (r < h->size && h->data[r] < h->data[l]) s = r;
        if (h->data[i] <= h->data[s]) break;
        seq_t t = h->data[i];
        h->data[i] = h->data[s];
        h->data[s] = t;
        i = s;
    }
    return ret;
}

/* ---------- Static storage for sequence terms and candidates ---------- */
static struct dyn_array terms;
static struct heap candidate_heap;

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    /* first call: initialise structures */
    if (terms.data == NULL) dyn_init(&terms);
    if (candidate_heap.data == NULL) heap_init(&candidate_heap);

    /* seed the heap with single‑digit candidates only once */
    if (candidate_heap.size == 0 && terms.len == 0) {
        heap_push(&candidate_heap, (seq_t)1);
        heap_push(&candidate_heap, (seq_t)4);
        heap_push(&candidate_heap, (seq_t)6);
        heap_push(&candidate_heap, (seq_t)8);
    }

    /* generate terms until we have at least n of them */
    while (terms.len < (size_t)n) {
        seq_t cur = heap_pop(&candidate_heap);

        /* generate children (append 1,4,6,8) */
        seq_t base = seq_mul(cur, (seq_t)10);
        heap_push(&candidate_heap, base + (seq_t)1);
        heap_push(&candidate_heap, base + (seq_t)4);
        heap_push(&candidate_heap, base + (seq_t)6);
        heap_push(&candidate_heap, base + (seq_t)8);

        /* test whether cur^2 uses only allowed digits */
        seq_t sq = seq_mul(cur, cur);
        if (!only_allowed(sq)) continue;

        /* cur is a valid term */
        dyn_push(&terms, cur);
    }

    return terms.data[n - 1];
}
