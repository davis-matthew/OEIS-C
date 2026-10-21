/*
 * A053198  Totients of consecutive pure powers of primes.
 * https://oeis.org/A053198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A025475(n+1))
 * Range:     u8 n <= 16, u16 n <= 93, u32 n <= 6948, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  i.e. a(n) = φ(p^k) where p^k (k>1) is the n-th pure prime power. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declarations of useful utilities if needed */
#include "../utils/build_primes_upto.h"

typedef struct {
    uint64_t power; /* current prime power */
    uint64_t prime; /* base prime */
} HeapNode;

/* static state */
static seq_t *seq_cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;          /* number of cached terms */
static size_t cache_size = 0;         /* amount allocated */

/* heap for generating prime powers in ascending order */
static HeapNode *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_size = 0;

/* list of primes */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Utility to ensure capacity of a generic array of seq_t */
static void ensure_seq_cap(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Utility to ensure heap capacity */
static void ensure_heap_cap(size_t needed) {
    if (needed <= heap_cap) return;
    size_t new_cap = heap_cap ? heap_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    HeapNode *new_heap = (HeapNode*)realloc(heap, new_cap * sizeof(HeapNode));
    if (!new_heap) abort();
    heap = new_heap;
    heap_cap = new_cap;
}

/* Basic binary min-heap operations */
static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

static void heap_up(size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[idx].power >= heap[parent].power) break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

static void heap_down(size_t idx) {
    while (true) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_size && heap[left].power < heap[smallest].power)
            smallest = left;
        if (right < heap_size && heap[right].power < heap[smallest].power)
            smallest = right;
        if (smallest == idx) break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

static void heap_push(HeapNode node) {
    ensure_heap_cap(heap_size + 1);
    heap[heap_size] = node;
    heap_up(heap_size);
    heap_size++;
}

static HeapNode heap_pop_min(void) {
    HeapNode ret = heap[0];
    heap[0] = heap[heap_size - 1];
    heap_size--;
    heap_down(0);
    return ret;
}

/* Initialisation: generate a sufficient list of primes and
   initialise the heap with p^2 for each prime. */
static void init_sequence(void) {
    if (prime_list) return;      /* already initialised */

    /* choose a bound large enough so that 10000 prime powers are generated.
       200000 gives more than enough for n <= 10000. */
    const idx_t prime_limit = 200000;
    build_primes_upto(prime_limit, &prime_list, &prime_cnt);

    /* initialise heap */
    heap_cap = prime_cnt;
    heap = (HeapNode*)malloc(heap_cap * sizeof(HeapNode));
    heap_size = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p <= 1) continue;
        /* p^2 */
        if (p > UINT64_MAX / p) continue; /* overflow guard */
        HeapNode node = { p * p, p };
        heap_push(node);
    }
}

/* Ensure the cache contains at least 'needed' terms */
static void extend_cache(size_t needed) {
    if (needed <= cache_size) return;
    init_sequence();

    ensure_seq_cap(needed);
    while (cache_size < needed) {
        HeapNode top = heap_pop_min();

        /* compute phi(p^k) = p^(k-1)*(p-1) = (power/p)*(p-1) */
        seq_t part = (seq_t)(top.power / top.prime);   /* p^(k-1) */
        part *= (top.prime - 1);                       /* multiply by (p-1) */
        seq_cache[cache_size++] = part;

        /* push next power for this prime if it does not overflow */
        if (top.power > 0 && top.prime > 0 && top.power <= UINT64_MAX / top.prime) {
            uint64_t next_power = top.power * top.prime;
            HeapNode next = { next_power, top.prime };
            heap_push(next);
        }
        /* otherwise the chain for this prime terminates */
    }
}

extern seq_t a(idx_t n) {
    if (n == 0) return 0; /* should not happen for offset 1, but safe */
    extend_cache((size_t)n);
    return seq_cache[n - 1];
}
