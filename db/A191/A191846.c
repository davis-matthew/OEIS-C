/*
 * A191846  Ordered sums 2*f+3*g, where f and g are Fibonacci numbers
 *          (A000045).
 * https://oeis.org/A191846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*F(i)+3*F(j) for i,j >= 1, sorted ascending, distinct
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


static struct Node {
    seq_t val;
    idx_t i;
    idx_t j;
} *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static seq_t *fib_cache = NULL;
static idx_t fib_size = 0;
static idx_t fib_cap = 0;

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* visited set for pairs (i,j) packed into a 64‑bit key: high 32 bits = i, low 32 bits = j */
#define VIS_CAP (1 << 18)
static uint64_t vis_table[VIS_CAP];
#define VIS_EMPTY 0

static inline bool vis_contains(uint64_t key) {
    size_t idx = key & (VIS_CAP - 1);
    for (;;) {
        uint64_t k = vis_table[idx];
        if (k == VIS_EMPTY) return false;
        if (k == key) return true;
        idx = (idx + 1) & (VIS_CAP - 1);
    }
}

static inline void vis_insert(uint64_t key) {
    size_t idx = key & (VIS_CAP - 1);
    for (;;) {
        uint64_t k = vis_table[idx];
        if (k == VIS_EMPTY) {
            vis_table[idx] = key;
            return;
        }
        idx = (idx + 1) & (VIS_CAP - 1);
    }
}

/* Fibonacci cache -------------------------------------------------------*/
static void ensure_fib(idx_t need) {
    while (fib_size <= need) {
        if (fib_size >= fib_cap) {
            fib_cap = fib_cap ? fib_cap * 2 : 4;
            fib_cache = (seq_t *)realloc(fib_cache, fib_cap * sizeof(seq_t));
        }
        fib_cache[fib_size] = fib_cache[fib_size - 1] + fib_cache[fib_size - 2];
        fib_size++;
    }
}

static inline seq_t get_fib(idx_t k) {
    ensure_fib(k);
    return fib_cache[k];
}

/* Min‑heap --------------------------------------------------------------*/
static void heap_init(void) {
    heap_cap = 4;
    heap = (struct Node *)malloc(heap_cap * sizeof(*heap));
    heap_size = 0;
}

static inline bool node_lt(const struct Node *a, const struct Node *b) {
    return (a->val < b->val) ||
           (a->val == b->val && ((a->i < b->i) ||
                                 (a->i == b->i && a->j < b->j)));
}

static void heap_percolate_up(size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (!node_lt(&heap[idx], &heap[parent]))
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[parent];
        heap[parent] = tmp;
        idx = parent;
    }
}

static void heap_percolate_down(size_t idx) {
    while (1) {
        size_t left = idx * 2 + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && node_lt(&heap[right], &heap[left]))
            smallest = right;
        if (!node_lt(&heap[smallest], &heap[idx]))
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

static void heap_push(seq_t val, idx_t i, idx_t j) {
    if (heap_size >= heap_cap) {
        heap_cap <<= 1;
        heap = (struct Node *)realloc(heap, heap_cap * sizeof(*heap));
    }
    heap[heap_size].val = val;
    heap[heap_size].i = i;
    heap[heap_size].j = j;
    heap_percolate_up(heap_size);
    heap_size++;
}

static struct Node heap_pop(void) {
    struct Node top = heap[0];
    heap[0] = heap[--heap_size];
    heap_percolate_down(0);
    return top;
}

static inline void ensure_cache_cap(size_t need) {
    while (cache_cap < need) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Function requested by the build system ---------------------------------*/
seq_t a(idx_t n) {
    static bool init = false;
    if (!init) {
        /* Fibonacci cache: 0 = 0, 1 = 1 */
        fib_cap = 4;
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_size = 2;

        heap_init();

        /* Insert the first pair (1,1) */
        uint64_t key = (((uint64_t)1) << 32) | 1;
        vis_insert(key);
        heap_push(get_fib(1) * (seq_t)2 + get_fib(1) * (seq_t)3, 1, 1);

        init = true;
    }

    if (n <= cache_size)
        return cache[n - 1];

    ensure_cache_cap(n + 10);

    while (cache_size < n) {
        struct Node cur = heap_pop();

        /* Store distinct values only */
        if (cache_size == 0 || !(cache[cache_size - 1] == cur.val)) {
            cache[cache_size++] = cur.val;
        }

        /* Child (i+1, j) */
        idx_t ci = cur.i + 1;
        idx_t cj = cur.j;
        uint64_t key = (((uint64_t)ci) << 32) | cj;
        if (!vis_contains(key)) {
            vis_insert(key);
            seq_t val = get_fib(ci) * (seq_t)2 + get_fib(cj) * (seq_t)3;
            heap_push(val, ci, cj);
        }

        /* Child (i, j+1) */
        ci = cur.i;
        cj = cur.j + 1;
        key = (((uint64_t)ci) << 32) | cj;
        if (!vis_contains(key)) {
            vis_insert(key);
            seq_t val = get_fib(ci) * (seq_t)2 + get_fib(cj) * (seq_t)3;
            heap_push(val, ci, cj);
        }
    }

    return cache[n - 1];
}
