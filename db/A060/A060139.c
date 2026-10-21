/*
 * A060139  Ordered set S defined by these rules: 0 and 1 are in S and if x is
 *          a nonzero number in S, then 3x-1 and 9x are in S.
 * https://oeis.org/A060139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate elements of the ordered set S defined by
   0,1 and the rules x∈S, x≠0 ⇒ 3x-1, 9x ∈ S.
   The elements are produced in increasing order by a binary min‑heap
   and stored in a static cache to allow random access.  The code works
   for any seq_t / idx_t combination that satisfies the constraints
   given in the problem statement. */

#include <stdlib.h>

/* --------------------- global state --------------------------------- */
static seq_t *cache = NULL;   /* cache of generated sequence values   */
static size_t cache_size = 0, cache_cap = 0;
static seq_t *heap  = NULL;   /* binary min‑heap of candidates */
static size_t heap_size = 0, heap_cap = 0;

/* --------------------- heap operations -------------------------------- */
static void heap_push(seq_t val)
{
    /* enlarge storage if necessary */
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 64;
        heap = (seq_t*)realloc(heap, heap_cap * sizeof(seq_t));
    }
    /* sift up */
    size_t i = heap_size++;
    heap[i] = val;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= heap[i]) break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static seq_t heap_pop(void)
{
    seq_t top = heap[0];
    seq_t last = heap[--heap_size];
    if (heap_size > 0) {
        heap[0] = last;
        /* sift down */
        size_t i = 0;
        while (1) {
            size_t left  = (i << 1) + 1;
            if (left >= heap_size) break;
            size_t right = left + 1;
            size_t smallest = left;
            if (right < heap_size && heap[right] < heap[left])
                smallest = right;
            if (heap[i] <= heap[smallest]) break;
            seq_t tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
    }
    return top;
}

/* --------------------- main sequence function ----------------------- */
seq_t a(idx_t n)
{
    /* lazy initialization of cache and heap */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;          /* a(0) = 0 */
        cache_size = 1;

        heap_cap = 64;
        heap = (seq_t*)malloc(heap_cap * sizeof(seq_t));
        heap_size = 0;
        heap_push((seq_t)1);          /* seed the heap with 1 */
    }

    /* generate terms until we have at least n+1 of them */
    while (cache_size <= (size_t)n) {
        seq_t x = heap_pop();
        /* skip duplicates that can appear from different parents */
        if (cache_size > 0 && x == cache[cache_size - 1])
            continue;

        /* store new term */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = x;

        if (x != (seq_t)0) {
            heap_push(3 * x - 1);
            heap_push(9 * x);
        }
    }

    return cache[n];
}
