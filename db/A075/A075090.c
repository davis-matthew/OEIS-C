/*
 * A075090  Even perfect powers: numbers of the form (2*m)^k for some m>=1 and
 *          k>=2.
 * https://oeis.org/A075090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075090: Even perfect powers – numbers of the form (2*m)^k,
   m ≥ 1, k ≥ 2, sorted increasingly with duplicates removed.
   The function generates terms by keeping a min‑heap of the current
   smallest power of each even base and repeatedly extracting the
   minimum, then inserting the next power of that base.
   A static cache stores the produced terms for O(1) lookup on
   subsequent calls.  The implementation uses only integer operations
   and works for any seq_t (built‑in or arbitrary‑precision). */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Configurable limits – enough for n up to about 10 000.            */
/* ------------------------------------------------------------------ */
#define MAX_CACHE_CAP 20000 /* maximum number of terms to store */
#define MAX_HEAP_CAP  40000 /* maximum number of simultaneous bases */

/* ------------------------------------------------------------------ */
/* Node stored in the heap – the current smallest power of a base.    */
/* ------------------------------------------------------------------ */
typedef struct {
    seq_t value; /* current power (base^k) */
    seq_t base;  /* the base (even number) */
} node_t;

/* ------------------------------------------------------------------ */
/* Helper functions for the binary min‑heap.                          */
/* ------------------------------------------------------------------ */
static inline void swap_node(node_t *a, node_t *b) {
    node_t t = *a;
    *a = *b;
    *b = t;
}

static void heapify_up(node_t *heap, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].value <= heap[idx].value)
            break;
        swap_node(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static void heapify_down(node_t *heap, size_t size, size_t idx) {
    while (1) {
        size_t left  = idx * 2 + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && heap[left].value < heap[smallest].value)
            smallest = left;
        if (right < size && heap[right].value < heap[smallest].value)
            smallest = right;
        if (smallest == idx)
            break;
        swap_node(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* ------------------------------------------------------------------ */
/* Static storage for the cache and the heap.                          */
/* ------------------------------------------------------------------ */
static seq_t  cache[MAX_CACHE_CAP];
static size_t cache_len = 0; /* number of terms currently stored */

static node_t heap[MAX_HEAP_CAP];
static size_t heap_len = 0;        /* current number of nodes in heap */

static idx_t max_base_included = 0; /* largest even base that has been pushed */

/* Keep track of the last value inserted into the cache – to skip
   duplicates that may arise when the same number is generated from
   different bases. */
static seq_t last_value;
static bool   has_last = false;

/* ------------------------------------------------------------------ */
/* Insert a new node into the heap.                                    */
/* ------------------------------------------------------------------ */
static void heap_push(node_t node) {
    heap[heap_len++] = node;
    heapify_up(heap, heap_len - 1);
}

/* Remove and return the smallest node from the heap.                  */
/* ------------------------------------------------------------------ */
static node_t heap_pop(void) {
    node_t ret = heap[0];
    heap[0] = heap[--heap_len];
    if (heap_len > 0)
        heapify_down(heap, heap_len, 0);
    return ret;
}

/* ------------------------------------------------------------------ */
/* Add all even bases up to 'limit' (inclusive) that have not yet been
   added to the heap.  For each, push the first power (base^2).         */
/* ------------------------------------------------------------------ */
static void add_bases_up_to(idx_t limit) {
    if (limit <= max_base_included)
        return;

    /* Ensure limit is even */
    if ((limit & 1ULL) != 0)
        --limit;

    for (idx_t b = max_base_included + 2; b <= limit; b += 2) {
        node_t n = { (seq_t)b * (seq_t)b, /* value = base^2 */
                     (seq_t)b };          /* base          */
        heap_push(n);
    }
    max_base_included = limit;
}

/* ------------------------------------------------------------------ */
/* Generate terms until we have at least 'need' of them.                */
/* ------------------------------------------------------------------ */
static void compute_until(idx_t need) {
    /* Roughly 2*need even bases are enough to produce 'need'
       distinct powers.  This keeps the heap size manageable. */
    add_bases_up_to(2 * need);

    while ((idx_t)cache_len < need) {
        node_t cur = heap_pop();

        /* Skip duplicates */
        if (!has_last || cur.value != last_value) {
            cache[cache_len++] = cur.value;
            last_value = cur.value;
            has_last = true;
        }

        /* Push the next power of the same base */
        node_t nxt = { cur.value * cur.base, cur.base };
        heap_push(nxt);
    }
}

/* ------------------------------------------------------------------ */
/* Public API – returns the n-th term (1‑based index).                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* offset is 1 – guard against out of bounds */

    /* Ensure the cache contains at least n terms */
    compute_until(n);

    return cache[n - 1];
}
