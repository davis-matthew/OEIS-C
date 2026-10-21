/*
 * A215344  Value of x in the n-th number of the form x+y*(1+sqrt(5))/2.
 * https://oeis.org/A215344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1571, u16 n <= 1571, u32 n <= 1571, u64 n <= 1571,
 *            u128 n <= 1571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A215344: Value of x in the n-th number of the form
   x + y*(1+sqrt(5))/2.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compiled in two phases: first on C11, then C++17.
   The types seq_t and idx_t are provided by the build system.  */

/* ----- constants ----------------------------------------------------- */
static const double PHI = 1.6180339887498948482;     /* (1+sqrt(5))/2 */

/* ----- visited set for (x,y) pairs ----------------------------------- */
#define VISIT_SIZE  (1 << 16)           /* must be power of two */
static uint64_t *visit_tbl = NULL;
static const uint64_t VISIT_EMPTY = 0xFFFFFFFFFFFFFFFFULL;

/* hash used for visited set */
static inline uint64_t hash_key(uint64_t k)
{
    return k * 11400714785074652221ULL;   /* Knuth's multiplicative hash */
}

/* insert returns true if key was not seen before */
static bool visit_insert(uint64_t key)
{
    size_t mask = VISIT_SIZE - 1;
    size_t i = hash_key(key) & mask;
    while (1) {
        uint64_t v = visit_tbl[i];
        if (v == VISIT_EMPTY) {
            visit_tbl[i] = key;
            return true;
        }
        if (v == key)
            return false;
        i = (i + 1) & mask;
    }
}

/* ----- min-heap for frontier nodes ----------------------------------- */
typedef struct {
    uint64_t x, y;
    double val;
} Node;

static Node *heap = NULL;
static size_t heap_len = 0, heap_cap = 0;

static inline void heap_reserve(size_t new_cap)
{
    if (new_cap > heap_cap) {
        heap_cap = new_cap > 16 ? new_cap : 16;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
}

static void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val)
            break;
        Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static void sift_down(size_t i)
{
    size_t last = heap_len - 1;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l > last) break;
        size_t r = l + 1;
        size_t c = (r <= last && heap[r].val < heap[l].val) ? r : l;
        if (heap[c].val >= heap[i].val)
            break;
        Node tmp = heap[c];
        heap[c] = heap[i];
        heap[i] = tmp;
        i = c;
    }
}

static void heap_push(Node n)
{
    heap_reserve(heap_len + 1);
    heap[heap_len++] = n;
    sift_up(heap_len - 1);
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_len];
    sift_down(0);
    return top;
}

/* ----- cache for computed sequence values ---------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static inline void cache_reserve(size_t new_cap)
{
    if (new_cap > cache_cap) {
        cache_cap = new_cap > 16 ? new_cap : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* ----- initialization -------------------------------------------------- */
static bool _initialized = false;

static void init(void)
{
    if (_initialized) return;
    _initialized = true;

    /* visited table */
    visit_tbl = (uint64_t *)malloc(VISIT_SIZE * sizeof(uint64_t));
    for (size_t i = 0; i < VISIT_SIZE; ++i) visit_tbl[i] = VISIT_EMPTY;

    /* cache and heap capacities */
    cache_cap = 16;  cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    heap_cap = 16;   heap = (Node *)malloc(heap_cap * sizeof(Node));

    /* push the very first pair (0,0) */
    uint64_t key0 = ((uint64_t)0 << 32) | (uint64_t)0;
    visit_insert(key0);
    heap_push((Node){0, 0, 0.0});
}

/* ----- helper to push a neighbour if unvisited ----------------------- */
static inline void try_push(uint64_t nx, uint64_t ny)
{
    uint64_t key = (nx << 32) | ny;
    if (visit_insert(key)) {
        double v = (double)nx + (double)ny * PHI;
        heap_push((Node){nx, ny, v});
    }
}

/* ----- the public function ------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    while (cache_len < n) {
        Node cur = heap_pop();
        cache_reserve(cache_len + 1);
        cache[cache_len++] = (seq_t)cur.x;

        try_push(cur.x + 1, cur.y);
        try_push(cur.x, cur.y + 1);
    }
    return cache[n - 1];
}
