/*
 * A201544  Odd numbers of the form x^2 + 2*y^2 with positive integers x and
 *          y.
 * https://oeis.org/A201544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd numbers of the form x^2 + 2*y^2 with positive integers x and y.
   Implementation performs a multi-way merge over all y = 1 … YMAX, keeping
   only odd x-values (x = 1,3,5,…).  Results are cached so that independent
   calls receive the correct term regardless of call order. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define YMAX 40000  /* enough to generate the first 10 000 terms */

typedef struct {
    seq_t val;          /* current value x^2 + 2*y^2 */
    uint64_t y;         /* fixed y of this sequence */
    uint64_t x;         /* current odd x */
} node_t;

static node_t *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_size = 0;

static inline void heap_swap(node_t *a, node_t *b) {
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(node_t n) {
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (node_t *)realloc(heap, heap_cap * sizeof(node_t));
    }
    size_t i = heap_size++;
    heap[i] = n;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[i].val >= heap[parent].val) break;
        heap_swap(&heap[i], &heap[parent]);
        i = parent;
    }
}

static node_t heap_pop(void) {
    node_t ret = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[i].val <= heap[smallest].val) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return ret;
}

static void init_heap(void) {
    if (heap != NULL) return;           /* already initialized */
    heap_cap = YMAX;
    heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    heap_size = 0;
    for (uint64_t y = 1; y <= YMAX; ++y) {
        node_t n;
        n.y = y;
        n.x = 1;                       /* start with odd x = 1 */
        n.val = (seq_t)1 + (seq_t)2 * (seq_t)y * (seq_t)y;
        heap_push(n);
    }
}

/* Cache of already generated terms for independent calls. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;         /* defined only for n ≥ 1 */

    init_heap();

    /* Ensure cache can hold n terms. */
    if ((size_t)n > cache_size) {
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < (size_t)n) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* Generate terms until we reach the nth one. */
        while (cache_size < (size_t)n) {
            node_t cur = heap_pop();

            /* Store the value only if it is a new (unique) term. */
            if (cache_size == 0 || cur.val != cache[cache_size - 1]) {
                cache[cache_size++] = cur.val;
            }

            /* Advance this y-sequence to the next odd x. */
            cur.x += 2;
            cur.val = (seq_t)cur.x * (seq_t)cur.x + (seq_t)2 * (seq_t)cur.y * (seq_t)cur.y;
            heap_push(cur);
        }
    }
    return cache[(size_t)n - 1];
}
