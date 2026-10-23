/*
 * A153775  Sequence S such that 1 is in S and if x is in S, then 3x-1 and 3x
 *          are in S.
 * https://oeis.org/A153775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153775
   Generation of the sequence defined by: 1∈S and for every x∈S
   also 3x−1∈S and 3x∈S.  The terms are produced in increasing order
   using a min‑heap of pending candidates.  The algorithm is O(n)
   memory times a small constant factor. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper functions for dynamic allocation that work in both C and C++. */
static void* seq_malloc(size_t elems)
{
#ifdef __cplusplus
    return static_cast<void*>(new seq_t[elems]);
#else
    return malloc(elems * sizeof(seq_t));
#endif
}
static void seq_free(seq_t* ptr, size_t /*elems*/)
{
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

/* Cache of generated terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;  /* capacity of the cache array */

/* Min‑heap of next candidates */
static seq_t *heap = NULL;
static size_t heap_len = 0;   /* number of elements in the heap */
static size_t heap_cap = 0;   /* capacity of the heap array */

/* Ensure the heap has room for one more element */
static void heap_grow(void)
{
    size_t new_cap = (heap_cap == 0) ? 16 : heap_cap << 1;
    seq_t *new_arr = (seq_t*)seq_malloc(new_cap);
    if (!new_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i < heap_len; ++i) new_arr[i] = heap[i];
    seq_free(heap, heap_cap);
    heap = new_arr;
    heap_cap = new_cap;
}

/* sift up after insertion */
static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= heap[i]) break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

/* sift down after removing the minimum */
static void heap_sift_down(size_t i)
{
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_len && heap[right] < heap[left]) smallest = right;
        if (heap[i] <= heap[smallest]) break;
        seq_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* insert a value into the heap */
static void heap_push(seq_t v)
{
    if (heap_len == heap_cap) heap_grow();
    heap[heap_len++] = v;
    heap_sift_up(heap_len - 1);
}

/* pop and return the smallest value from the heap */
static seq_t heap_pop_min(void)
{
    seq_t min = heap[0];
    if (--heap_len > 0) {
        heap[0] = heap[heap_len];
        heap_sift_down(0);
    }
    return min;
}

/* Ensure the cache can hold at least n terms */
static void cache_ensure(idx_t n)
{
    if (n <= cache_len) return;

    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)seq_malloc(cache_cap);
        if (!cache) exit(EXIT_FAILURE);
    }
    while (cache_len < n) {
        /* grow cache array if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *new_arr = (seq_t*)seq_malloc(new_cap);
            if (!new_arr) exit(EXIT_FAILURE);
            for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
            seq_free(cache, cache_cap);
            cache = new_arr;
            cache_cap = new_cap;
        }
        /* Normal initialization: push the root 1 the first time */
        if (cache_len == 0 && heap_len == 0) heap_push((seq_t)1);
        /* Generate next term */
        seq_t x = heap_pop_min();
        cache[cache_len++] = x;
        heap_push((seq_t)3 * x - (seq_t)1);
        heap_push((seq_t)3 * x);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* caller guarantees n ≥ 1, safety guard */
    cache_ensure(n);
    return cache[n - 1];
}
