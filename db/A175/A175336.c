/*
 * A175336  a(1)=1. For n>1, a(n) equals the smallest number > a(n-1) of the
 *          form a(k) U j U a(k) U j, where U represents concatenation
 *          (written in decimal) of the binary representation of the
 *          arguments, where 1<=k < n, and j = {0} or {1} or {}.
 * https://oeis.org/A175336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   bitlen_seq_t, heap_swap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 1, a(n) is the smallest integer > a(n‑1) that can be
   expressed as a(k) U j U a(k) U j,
   where 1 ≤ k < n and j ∈ {∅, 0, 1} (U denotes binary concatenation). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/heap_swap.h"

/* Min‑heap for seq_t values */
typedef struct {
    seq_t *data;
    size_t size;
    size_t capacity;
} seq_heap_t;

static void heap_init(seq_heap_t *h) {
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

static void heap_grow(seq_heap_t *h, size_t needed) {
    if (h->capacity >= needed) return;
    size_t newcap = h->capacity ? h->capacity * 2 : 8;
    while (newcap < needed) newcap <<= 1;
    h->data = (seq_t *)realloc(h->data, newcap * sizeof(seq_t));
    h->capacity = newcap;
}

static void heap_push(seq_heap_t *h, seq_t v) {
    heap_grow(h, h->size + 1);
    h->data[h->size++] = v;
    /* sift up */
    size_t i = h->size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= h->data[i]) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

static bool heap_empty(const seq_heap_t *h) {
    return h->size == 0;
}

static seq_t heap_pop(seq_heap_t *h) {
    seq_t res = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        size_t i = 0;
        while (true) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < h->size && h->data[l] < h->data[smallest]) smallest = l;
            if (r < h->size && h->data[r] < h->data[smallest]) smallest = r;
            if (smallest == i) break;
            heap_swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        }
    }
    return res;
}

/* Dynamic array for the sequence */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void seq_grow(void) {
    size_t newcap = seq_cap ? seq_cap * 2 : 8;
    seq_arr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Add all candidates derived from a_k into the heap */
static void add_candidates(seq_heap_t *h, seq_t a_k) {
    idx_t len_a = bitlen_seq_t(a_k);            /* number of bits of a_k */
    /* j = ∅ : a_k U a_k */
    seq_t cand = (a_k << len_a) | a_k;
    heap_push(h, cand);

    /* j = 0 : a_k 0 a_k 0 */
    seq_t part = a_k << 1;                      /* append 0 */
    idx_t lpart = len_a + 1;
    cand = (part << lpart) | part;
    heap_push(h, cand);

    /* j = 1 : a_k 1 a_k 1 */
    part = (a_k << 1) | (seq_t)1;              /* append 1 */
    cand = (part << lpart) | part;
    heap_push(h, cand);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for 0, return 0 */

    size_t target = (size_t)n;

    if (seq_len == 0) {
        seq_grow();
        seq_arr[0] = (seq_t)1;              /* a(1) */
        seq_len = 1;
    }

    /* Lazy generation */
    static seq_heap_t heap;
    static bool heap_initialised = false;
    if (!heap_initialised) {
        heap_init(&heap);
        add_candidates(&heap, seq_arr[0]);  /* candidates from a(1) */
        heap_initialised = true;
    }

    while (seq_len < target) {
        if (heap_empty(&heap)) return (seq_t)0;    /* shouldn't happen */

        seq_t cand = heap_pop(&heap);
        seq_t last = seq_arr[seq_len - 1];
        if (cand <= last) continue;               /* discard <= previous */

        /* Accept candidate as new term */
        if (seq_len == seq_cap) seq_grow();
        seq_arr[seq_len++] = cand;

        /* Generate new candidates from this term */
        add_candidates(&heap, cand);
    }

    return seq_arr[seq_len - 1];
}
