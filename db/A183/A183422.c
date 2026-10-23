/*
 * A183422  Ordering of the numbers in the tree A183420; complement of
 *          A183423.
 * https://oeis.org/A183422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183422 – monotonic ordering of the set S generated
 *   by the rules: 2∈S; if n∈S then n²+4n+2∈S and n+⌊½+√(n+2)⌋∈S.
 * The sequence a(n) is the n‑th smallest element of the sorted set S. */

/* Standard C headers ---------------------------------------------------- */
#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* malloc, realloc, calloc, free, abort */

/* Helper for integer square root --------------------------------------- */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* ---------- Min‑heap for generating the terms ------------------------ */
static seq_t *heap = NULL;       /* heap array */
static size_t heap_cap = 0;      /* heap capacity */
static size_t heap_size = 0;     /* number of elements in the heap */

/* Swap two heap positions                                         */
static void heap_swap(size_t i, size_t j)
{
    seq_t tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* Push a new value onto the heap                                   */
static void heap_push(seq_t val)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(heap, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        heap_swap(i, p);
        i = p;
    }
}

/* Pop the smallest element from the heap                            */
static seq_t heap_pop(void)
{
    if (heap_size == 0) abort();
    seq_t min = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        size_t s = i;
        if (l < heap_size && heap[l] < heap[s]) s = l;
        if (r < heap_size && heap[r] < heap[s]) s = r;
        if (s == i) break;
        heap_swap(i, s);
        i = s;
    }
    return min;
}

/* ---------- Cache for already computed sequence values ------------- */
static seq_t *cache = NULL;        /* cache array */
static size_t cache_cap = 0;       /* capacity */
static size_t cache_len = 0;       /* filled elements */

/* Ensure the cache can hold at least 'needed' elements               */
static void cache_ensure(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Helper: round(√(n+2)) ----------------------------------- */
static seq_t round_sqrt_plus_2(seq_t n)
{
    /* s = floor(√(n+2)) */
    seq_t s = floor_sqrt(n + (seq_t)2);
    /* test whether we need to round up: s² + s + 1 ≤ n+2 < (s+1)² + (s+1) + 1 */
    seq_t threshold = s * s + s + (seq_t)1;
    return (n + (seq_t)2 >= threshold) ? (s + (seq_t)1) : s;
}

/* ---------- Public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is undefined */

    /* initialize heap once */
    static int heap_inited = 0;
    if (!heap_inited) {
        heap_push((seq_t)2);
        heap_inited = 1;
    }

    /* initialize cache */
    if (!cache) {
        cache = (seq_t*)calloc(64, sizeof(seq_t));
        if (!cache) abort();
        cache_cap = 64;
        cache_len = 0;
    }

    /* generate terms until we have at least n values in the cache */
    while (cache_len < (size_t)n) {
        seq_t val = heap_pop();

        /* skip duplicates that may appear consecutively */
        if (cache_len > 0 && val == cache[cache_len - 1]) continue;

        /* store the new distinct value */
        cache_ensure(cache_len + 1);
        cache[cache_len++] = val;

        /* generate the two children for the next rounds */
        seq_t child1 = val * val + (seq_t)4 * val + (seq_t)2;
        heap_push(child1);

        seq_t child2 = val + round_sqrt_plus_2(val);
        heap_push(child2);
    }

    return cache[n - 1];
}
