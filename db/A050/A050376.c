/*
 * A050376  "Fermi-Dirac primes": numbers of the form p^(2^k) where p is prime
 *          and k >= 0.
 * https://oeis.org/A050376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fermi‑Dirac primes – numbers of the form p^(2^k) (p prime, k≥0), sorted in ascending order */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(idx_t) */

typedef struct {
    seq_t val;
} HeapNode;

static HeapNode *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;
static idx_t next_prime_candidate = 2;
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;

/* ---------- Min‑heap operations ---------- */
static void heap_init(void) {
    heap_cap = 64;
    heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
    heap_size = 0;
}

static void heap_resize(void) {
    heap_cap *= 2;
    heap = (HeapNode *)realloc(heap, heap_cap * sizeof(HeapNode));
}

static void heap_push(seq_t value) {
    if (!heap) heap_init();
    if (heap_size == heap_cap) heap_resize();
    size_t i = heap_size++;
    heap[i].val = value;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static bool heap_empty(void) { return heap_size == 0; }

static seq_t heap_peek(void) { return heap[0].val; }

static seq_t heap_pop(void) {
    seq_t val = heap[0].val;
    heap_size--;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t left = (i << 1) + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left < heap_size && heap[left].val < heap[smallest].val) smallest = left;
            if (right < heap_size && heap[right].val < heap[smallest].val) smallest = right;
            if (smallest == i) break;
            HeapNode tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
    }
    return val;
}

/* ---------- Cache construction ---------- */
static void ensure_cache(size_t n) {
    if (cache_size >= n) return;
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_size + (cache_cap - cache_size) < n) {
        cache_cap *= 2;
    }
    if (cache_cap > cache_size) {
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    while (cache_size < n) {
        /* Insert enough primes so that the smallest uninserted prime
           is larger than the current heap minimum. */
        while (1) {
            if (heap_empty()) {
                /* No primes yet – find the next prime. */
                while (!is_prime(next_prime_candidate)) ++next_prime_candidate;
                heap_push((seq_t)next_prime_candidate);
                ++next_prime_candidate;
                continue;
            }

            seq_t heap_min = heap_peek();
            if ((next_prime_candidate <= heap_min) &&
                (next_prime_candidate > 0)) {
                if (is_prime(next_prime_candidate))
                    heap_push((seq_t)next_prime_candidate);
                ++next_prime_candidate;
                continue;
            }
            break; /* ready to pop */
        }

        seq_t cur = heap_pop();
        cache[cache_size++] = cur;

        /* Compute next power: cur^(2) = cur*cur, avoiding overflow. */
        if (cur != 0) {
            seq_t next = cur * cur;
            /* Detect overflow: if division restarts the original value,
               we stayed within bounds. */
            if (next / cur == cur) {
                heap_push(next);
            }
        }
    }
}

/* ---------- Public API ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, just return 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
