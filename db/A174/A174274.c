/*
 * A174274  Each pair of adjacent digits of n sums to a square.
 * https://oeis.org/A174274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th smallest non‑negative integer whose every
 * adjacent pair of decimal digits sums to a perfect square (0, 1, 4, 9, or 16). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* adjacency table: for each last digit d, adj[d] lists all digits x
   such that d + x is a square.  The unused slots are filled with 255. */
static const uint8_t adj[10][10] = {
    {0, 1, 4, 9, 255, 255, 255, 255, 255, 255},
    {0, 3, 8, 255, 255, 255, 255, 255, 255, 255},
    {2, 7, 255, 255, 255, 255, 255, 255, 255, 255},
    {1, 6, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 5, 255, 255, 255, 255, 255, 255, 255, 255},
    {4, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    {3, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    {2, 9, 255, 255, 255, 255, 255, 255, 255, 255},
    {1, 8, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 7, 255, 255, 255, 255, 255, 255, 255, 255}
};
static const uint8_t adj_len[10] = {4, 3, 2, 2, 2, 1, 1, 2, 2, 2};

/* --- heap implementation ------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> heap;

static inline void heap_push(seq_t val)
{
    heap.push_back(val);
    size_t i = heap.size() - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        std::swap(heap[p], heap[i]);
        i = p;
    }
}

static inline seq_t heap_pop(void)
{
    if (heap.empty()) return 0;
    seq_t top = heap[0];
    seq_t last = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        heap[0] = last;
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap.size() && heap[l] < heap[smallest]) smallest = l;
            if (r < heap.size() && heap[r] < heap[smallest]) smallest = r;
            if (smallest == i) break;
            std::swap(heap[i], heap[smallest]);
            i = smallest;
        }
    }
    return top;
}

static std::vector<seq_t> cache;
#else
static seq_t *heap_data = NULL;
static size_t heap_size = 0, heap_cap = 0;

static inline void heap_push(seq_t val)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(heap_data, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        heap_data = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap_data[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_data[p] <= heap_data[i]) break;
        seq_t t = heap_data[p];
        heap_data[p] = heap_data[i];
        heap_data[i] = t;
        i = p;
    }
}

static inline seq_t heap_pop(void)
{
    if (heap_size == 0) return 0;
    seq_t top = heap_data[0];
    seq_t last = heap_data[--heap_size];
    if (heap_size > 0) {
        heap_data[0] = last;
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_size && heap_data[l] < heap_data[smallest]) smallest = l;
            if (r < heap_size && heap_data[r] < heap_data[smallest]) smallest = r;
            if (smallest == i) break;
            seq_t t = heap_data[i];
            heap_data[i] = heap_data[smallest];
            heap_data[smallest] = t;
            i = smallest;
        }
    }
    return top;
}

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
#endif

#ifndef __cplusplus
static const seq_t seq_t_max = ~(seq_t)0;
#endif

static inline void cache_push(seq_t val)
{
#ifdef __cplusplus
    cache.push_back(val);
#else
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static bool initialized = false;
    if (!initialized) {
        /* seed heap with single‑digit numbers 1–9 */
        for (int d = 1; d <= 9; ++d) heap_push((seq_t)d);
        initialized = true;
    }

    /* generate terms until we have at least n values */
#ifdef __cplusplus
    while (cache.size() < (size_t)n) {
#else
    while (cache_len < (size_t)n) {
#endif
        seq_t val = heap_pop();
        if (val == 0) break;   /* heap exhausted */

        if (val >= (seq_t)10) cache_push(val);

        /* generate children */
#ifndef __cplusplus
        if (val > seq_t_max / 10) continue;
#endif
        uint8_t last = (uint8_t)(val % (seq_t)10);
        const uint8_t *lst = adj[last];
        size_t len = adj_len[last];
        for (size_t i = 0; i < len; ++i) {
            seq_t child = val * (seq_t)10 + (seq_t)lst[i];
            heap_push(child);
        }
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
