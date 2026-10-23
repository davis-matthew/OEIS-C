/*
 * A162142  Numbers that are the cube of a product of two distinct primes
 *          (p^3*q^3).
 * https://oeis.org/A162142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p*q)^3 where p,q are primes, p<q (sorted by product)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 454, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* ----------------------------------- */
/* Prime generation (once)               */
/* ----------------------------------- */
static uint64_t *primes      = NULL;
static size_t    prime_count  = 0;
static bool      primes_done  = false;

static void compute_primes(void)
{
    if (primes_done) return;

    const idx_t LIMIT = 200000;                       /* ample prime pool   */
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));

    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            ++cnt;
            if (i * i <= LIMIT)
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = true;
        }
    }

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) primes[pos++] = i;

    prime_count = cnt;
    primes_done = true;
    free(sieve);
}

/* ----------------------------------- */
/* Heap for generating sorted products  */
/* ----------------------------------- */
typedef struct {
    uint64_t prod;   /* p[i] * p[j] (fits in 64‑bit) */
    size_t   i;      /* first prime index      */
    size_t   j;      /* second prime index     */
} HeapNode;

static HeapNode *heap      = NULL;
static size_t   heap_size  = 0;   /* current number of elements */

/* Sink down to maintain min‑heap property */
static void sift_down(size_t idx)
{
    while (1) {
        size_t left  = idx * 2 + 1;
        size_t right = idx * 2 + 2;
        size_t smallest = idx;

        if (left < heap_size && heap[left].prod < heap[smallest].prod)
            smallest = left;
        if (right < heap_size && heap[right].prod < heap[smallest].prod)
            smallest = right;

        if (smallest == idx) break;
        HeapNode tmp = heap[idx];
        heap[idx]  = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

/* Push a new node onto the heap */
static void heap_push(HeapNode node)
{
    heap[heap_size++] = node;
    size_t idx = heap_size - 1;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].prod <= heap[idx].prod) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* Pop the smallest node from the heap */
static HeapNode heap_pop(void)
{
    HeapNode top = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return top;
}

/* ----------------------------------- */
/* Cache for generated sequence values  */
/* ----------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of values stored */
static size_t cache_cap  = 0;

/* Ensure cache has space for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 512;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ----------------------------------- */
/* Public sequence entry point          */
/* ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined */

    /* Prime and heap initialization on first use */
    if (prime_count == 0) {
        compute_primes();

        /* Initialize heap with (p[i], p[i+1]) for all i */
        heap_size = prime_count - 1;
        heap = (HeapNode *)malloc(heap_size * sizeof(HeapNode));
        for (size_t i = 0; i + 1 < prime_count; ++i) {
            heap[i].prod = primes[i] * primes[i + 1];
            heap[i].i    = i;
            heap[i].j    = i + 1;
        }
        /* Build min‑heap */
        for (int k = (int)(heap_size / 2) - 1; k >= 0; --k)
            sift_down((size_t)k);
    }

    /* Generate additional terms until we have at least n entries */
    while (cache_len < n) {
        HeapNode cur = heap_pop();

        /* Compute (p[i]*p[j])^3 */
        seq_t prod   = (seq_t)primes[cur.i] * (seq_t)primes[cur.j];
        seq_t value  = big_pow(prod, (seq_t)3);

        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = value;

        /* Push next product for the same first prime, if any */
        size_t next_j = cur.j + 1;
        if (next_j < prime_count) {
            HeapNode next;
            next.prod = primes[cur.i] * primes[next_j];
            next.i    = cur.i;
            next.j    = next_j;
            heap_push(next);
        }
        /* If next_j == prime_count, this row is exhausted; heap_size already shrunk */
    }

    return cache[n - 1];
}
