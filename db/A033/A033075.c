/*
 * A033075  Positive numbers all of whose pairs of consecutive decimal digits
 *          differ by 1.
 * https://oeis.org/A033075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 187, u32 n <= 2361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   swap_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A033075: 10‑esthetic numbers (positive integers whose
   consecutive decimal digits differ by 1).  The function returns the
   n‑th term (1‑based), computed lazily and cached. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/swap_seq.h"

/* Utility macro to cast literals to seq_t. */
#define SEQ(x) ((seq_t)(x))

/* Initial heap/cache capacities. */
#define INIT_CAP 16

/* ----- Cached results ----- */
static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_len = 0;  /* number of terms cached */
static size_t cache_cap = 0;  /* allocated capacity */

/* ----- Min‑heap of frontier numbers ----- */
static seq_t *heap = NULL;     /* min‑heap array */
static size_t heap_size = 0;   /* current number of elements in heap */
static size_t heap_cap = 0;    /* allocated heap capacity */

/* ----- Ensure the heap has space for 'need' elements ----- */
static void heap_ensure_capacity(size_t need)
{
    if (heap_cap >= need)
        return;
    size_t new_cap = heap_cap > 0 ? heap_cap * 2 : INIT_CAP;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_heap = (seq_t *)realloc(heap, new_cap * sizeof(seq_t));
    if (!new_heap)
        abort();
    heap = new_heap;
    heap_cap = new_cap;
}

/* ----- Heap push (insert) ----- */
static void heap_push(seq_t v)
{
    heap_ensure_capacity(heap_size + 1);
    size_t pos = heap_size++;
    heap[pos] = v;
    /* bubble up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos] < heap[parent]) {
            swap_seq(&heap[pos], &heap[parent]);
            pos = parent;
        } else
            break;
    }
}

/* ----- Heap pop (remove minimum) ----- */
static seq_t heap_pop_min(void)
{
    seq_t minv = heap[0];
    heap[0] = heap[--heap_size];
    /* bubble down */
    size_t pos = 0;
    while (true) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap_size && heap[left] < heap[smallest])
            smallest = left;
        if (right < heap_size && heap[right] < heap[smallest])
            smallest = right;
        if (smallest == pos)
            break;
        swap_seq(&heap[pos], &heap[smallest]);
        pos = smallest;
    }
    return minv;
}

/* ----- Ensure the cache has space for 'need' terms ----- */
static void cache_ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : INIT_CAP;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ----- Generate the next 10‑esthetic number and store it ----- */
static void generate_next(void)
{
    seq_t val = heap_pop_min();            /* take smallest frontier */
    cache_ensure_capacity(cache_len + 1);  /* grow cache if necessary */
    cache[cache_len++] = val;              /* store result */

    /* Generate child numbers by appending a digit that differs by 1. */
    seq_t last = val % SEQ(10);
    if (last > SEQ(0))
        heap_push(val * SEQ(10) + (last - SEQ(1))); /* last-1 */
    if (last < SEQ(9))
        heap_push(val * SEQ(10) + (last + SEQ(1))); /* last+1 */
}

/* ----- One‑time initialization of heap and cache ----- */
static void initialize(void)
{
    /* Build the initial frontier heap with digits 1…9. */
    heap_cap = INIT_CAP;
    heap = (seq_t *)malloc(heap_cap * sizeof(seq_t));
    if (!heap)
        abort();
    heap_size = 0;
    for (seq_t d = SEQ(1); d <= SEQ(9); ++d)
        heap_push(d);

    /* Prepare empty cache. */
    cache_cap = INIT_CAP;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        abort();
    cache_len = 0;
}

/* ----- Public function: a(n) ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return SEQ(0);

    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we reach the requested index. */
    while (cache_len < n)
        generate_next();

    return cache[n - 1];
}
