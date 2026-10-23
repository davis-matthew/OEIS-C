/*
 * A155946  Numbers d for which the volume of the regular d-dimensional
 *          simplex of unit edge is rational.
 * https://oeis.org/A155946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A155946 – d such that the volume of the unit‐edge regular d‑simplex is rational.
   d = q²*2ᵏ – 1, where q is odd and k = 0 or k is odd.  The sequence is
   already sorted in ascending order. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* heap node: current d, the fixed odd q, and whether it belongs to the
   odd‑k branch (true) or not (false).  Only odd‑k nodes generate further
   values via d*4+3. */
typedef struct {
    seq_t d;
    idx_t q;
    bool odd_branch;
} HeapNode;

typedef struct {
    HeapNode *data;
    size_t size;
    size_t capacity;
} MinHeap;

/* ---------- heap helpers (C++ compatible casting) ---------- */
static void heap_init(MinHeap *h)
{
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

static void heap_resize(MinHeap *h, size_t new_cap)
{
    HeapNode *new_data = (HeapNode *)realloc(h->data, new_cap * sizeof(HeapNode));
    if (!new_data) exit(EXIT_FAILURE);
    h->data = new_data;
    h->capacity = new_cap;
}

static void heap_push(MinHeap *h, HeapNode node)
{
    if (h->size == h->capacity) {
        size_t new_cap = (h->capacity == 0) ? 16 : h->capacity * 2;
        heap_resize(h, new_cap);
    }
    size_t i = h->size++;
    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h->data[parent].d <= node.d) break;
        h->data[i] = h->data[parent];
        i = parent;
    }
    h->data[i] = node;
}

static HeapNode heap_pop_min(MinHeap *h)
{
    HeapNode min = h->data[0];
    HeapNode last = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= h->size) break;
        size_t right = left + 1;
        size_t child = (right < h->size && h->data[right].d < h->data[left].d) ? right : left;
        if (last.d <= h->data[child].d) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return min;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Static cache of produced terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static heap that keeps the next candidates from all odd q’s */
    static MinHeap heap;
    static bool heap_inited = false;

    /* Next odd q to be added after each pop (q=1 handled separately) */
    static idx_t next_q = 3;  /* odd sequence 1,3,5,...; 1 already seeded */

    /* Initialise heap once */
    if (!heap_inited) {
        heap_init(&heap);
        heap_inited = true;

        /* Seed q=1 (odd) values: k=0 and k=1 (odd branch) */
        seq_t sq = (seq_t)1 * (seq_t)1;          /* 1² */
        heap_push(&heap, (HeapNode){ sq - 1, 1, false });  /* d = 0 */
        heap_push(&heap, (HeapNode){ sq * 2 - 1, 1, true });/* d = 1 */
    }

    /* Grow cache to accommodate nth term */
    if (cache_len < n) {
        if (cache_len == 0) {             /* first allocation */
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        }

        while (cache_len < n) {
            /* Pop smallest candidate */
            HeapNode node = heap_pop_min(&heap);

            /* Store the value */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_len++] = node.d;

            /* If node came from odd branch (k>=1), generate next in that branch */
            if (node.odd_branch) {
                seq_t next_d = node.d * (seq_t)4 + (seq_t)3; /* d*4+3 */
                heap_push(&heap, (HeapNode){ next_d, node.q, true });
            }

            /* Add a new odd q (q=next_q) to provide more candidates */
            idx_t q_new = next_q;
            next_q += 2;                  /* keep odd numbers */

            seq_t sq_new = (seq_t)q_new * (seq_t)q_new;
            heap_push(&heap, (HeapNode){ sq_new - 1, q_new, false });   /* k=0 */
            heap_push(&heap, (HeapNode){ sq_new * 2 - 1, q_new, true });/* k=1 odd branch */
        }
    }

    return cache[n - 1];
}
