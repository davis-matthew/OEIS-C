/*
 * A229125  Numbers of the form p * m^2, where p is prime and m > 0: union of
 *          A228056 and A000040.
 * https://oeis.org/A229125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form p * m^2, where p is prime, m > 0
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/grow_seq.h"

typedef struct {
    seq_t val;     /* p * m^2 */
    uint64_t p;    /* prime p */
    uint32_t m;    /* current m (starting at 1) */
} Node;

/* ---------- heap routines for Node ---------- */
static void sift_up(Node *heap, size_t heap_size, size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static void sift_down(Node *heap, size_t heap_size, size_t i)
{
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = i;
        if (heap[left].val < heap[smallest].val) smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val) smallest = right;
        if (smallest == i) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(Node *heap, size_t *heap_size, const Node *n)
{
    heap[*heap_size] = *n;
    sift_up(heap, *heap_size, *heap_size);
    (*heap_size)++;
}

static void heap_pop(Node *heap, size_t *heap_size, Node *out)
{
    *out = heap[0];
    (*heap_size)--;
    if (*heap_size > 0) {
        heap[0] = heap[*heap_size];
        sift_down(heap, *heap_size, 0);
    }
}

/* ---------- prime sieve (up to 200000) ---------- */
#define PRIME_LIMIT 200000
static uint64_t *primes = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    bool *mark = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!mark) abort();
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!mark[i]) {
            if (prime_count == 0) {
                primes = (uint64_t *)malloc(PRIME_LIMIT * sizeof(uint64_t));
                if (!primes) abort();
            }
            primes[prime_count++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i) mark[j] = true;
            }
        }
    }
    free(mark);
}

/* ---------- global static state ---------- */
static bool initialized = false;
static size_t heap_size = 0;
static Node *heap = NULL;
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void seq_init(void)
{
    init_primes();
    heap = (Node *)malloc(prime_count * sizeof(Node));
    if (!heap) abort();
    for (size_t i = 0; i < prime_count; ++i) {
        heap[i].p = primes[i];
        heap[i].m = 1;
        heap[i].val = (seq_t)primes[i];  /* p * 1^2 */
    }
    heap_size = prime_count;
    /* build min‑heap */
    for (size_t i = (heap_size >> 1); i-- > 0; ) sift_down(heap, heap_size, i);
    initialized = true;
}

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    if (!initialized) seq_init();
    int required = (int)n;
    while ((int)cache_size < required) {
        Node top;
        heap_pop(heap, &heap_size, &top);
        seq_t val = top.val;
        if (cache_size == 0 || cache[cache_size - 1] != val) {
            if (cache_size >= cache_cap) grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = val;
        }
        uint32_t mnext = top.m + 1;
        seq_t new_val = (seq_t)top.p * (seq_t)mnext * (seq_t)mnext;
        Node next = { .val = new_val, .p = top.p, .m = mnext };
        heap_push(heap, &heap_size, &next);
    }
    return cache[required - 1];
}
