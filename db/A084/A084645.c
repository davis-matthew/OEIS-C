/*
 * A084645  Hypotenuses for which there exists a unique integer-sided right
 *          triangle.
 * https://oeis.org/A084645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer having exactly one prime divisor ≡1 (mod
 *            4) with exponent one
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate all primes ≡1 (mod 4) up to a fixed bound, build list M of integers not
 * divisible by any prime ≡1 (mod 4), then merge sequences p * M[mi] for all such primes p using a
 * min‑heap, caching results for subsequent calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* constants */
#define BOUND 10000000ULL  /* sieve limit, sufficient for n≈10000 */

/* node for min‑heap */
typedef struct {
    seq_t value;          /* current product p * M[mi] */
    size_t pi;            /* index of prime in primes array */
    size_t mi;            /* index in M array */
} HeapNode;

/* global data */
static uint32_t *primes = NULL;          /* primes ≡1 (mod 4) */
static size_t primes_cnt = 0;

static size_t *M = NULL;                /* array of numbers without factors ≡1 (mod 4) */
static size_t M_cnt = 0;

static HeapNode *heap = NULL;           /* min‑heap */
static size_t heap_size = 0;

static seq_t *cache = NULL;             /* cached sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* utility: sifting down in a min‑heap */
static void sift_down(HeapNode *h, size_t size, size_t idx) {
    while (1) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < size && h[left].value < h[smallest].value) smallest = left;
        if (right < size && h[right].value < h[smallest].value) smallest = right;
        if (smallest == idx) break;
        HeapNode tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

/* build sieve of primes up to BOUND, collect primes ≡1 (mod 4) */
static void init_primes(void) {
    /* allocate buffers */
    size_t sieve_size = BOUND + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sieve_size);
    if (!is_prime) return;
    for (size_t i = 0; i < sieve_size; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* simple sieve */
    size_t limit = 1;
    while ((limit + 1) * (limit + 1) <= BOUND) limit++;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) {
        for (size_t j = i * i; j <= BOUND; j += i) is_prime[j] = 0;
    }

    /* count primes ≡1 (mod 4) to allocate */
    primes_cnt = 0;
    for (size_t i = 5; i <= BOUND; i += 4) if (is_prime[i]) primes_cnt++;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }

    /* store primes */
    size_t idx = 0;
    for (size_t i = 5; i <= BOUND; i += 4) if (is_prime[i]) primes[idx++] = (uint32_t)i;
    free(is_prime);
}

/* build array M of integers ≤ BOUND that are not divisible by any prime ≡1 (mod 4) */
static void init_M(void) {
    size_t size = BOUND + 1;
    unsigned char *bad = (unsigned char *)calloc(size, 1);
    if (!bad) return;

    /* mark multiples of each prime ≡1 (mod 4) */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        for (size_t j = p; j <= BOUND; j += p) bad[j] = 1;
    }

    /* count good numbers to allocate M */
    M_cnt = 0;
    for (size_t i = 1; i <= BOUND; ++i) if (!bad[i]) M_cnt++;

    M = (size_t *)malloc(M_cnt * sizeof(size_t));
    if (!M) { free(bad); return; }

    size_t pos = 0;
    for (size_t i = 1; i <= BOUND; ++i) if (!bad[i]) M[pos++] = i;

    free(bad);
}

/* initialize data structures once */
static void init_sequence(void) {
    init_primes();
    init_M();

    heap_size = primes_cnt;
    heap = (HeapNode *)malloc(heap_size * sizeof(HeapNode));
    if (!heap) return;

    /* initial heap nodes */
    for (size_t i = 0; i < heap_size; ++i) {
        heap[i].pi = i;
        heap[i].mi = 0;
        heap[i].value = (seq_t)primes[i] * (seq_t)M[0];
    }

    /* build min‑heap */
    for (size_t i = (heap_size >> 1); i-- > 0; ) sift_down(heap, heap_size, i);
}

/* the sequence function */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* generate terms until we reach nth */
    while (cache_len < (size_t)n) {
        /* get next value from heap */
        seq_t val = heap[0].value;

        /* append to cache */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 128;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;

        /* advance the prime in the heap */
        heap[0].mi++;
        heap[0].value = (seq_t)primes[heap[0].pi] * (seq_t)M[heap[0].mi];
        sift_down(heap, heap_size, 0);
    }

    return cache[(size_t)(n - 1)];
}
