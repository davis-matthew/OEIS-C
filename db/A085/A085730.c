/*
 * A085730  Euler's totient function applied to the sequence of prime powers.
 * https://oeis.org/A085730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ( prime_power(n) ) (Euler's totient of the nth prime
 *            power; a(1)=φ(1)=1)
 * Range:     u8 n <= 71, u16 n <= 6636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ----------------------------------------------------------- */
/* Basic heap of prime powers                                            */
typedef struct {
    uint64_t val;     /* current value p^e                 */
    uint64_t prime;   /* base prime p                      */
    size_t   exp;     /* exponent e                        */
} HeapNode;

static HeapNode *heap = NULL;      /* the heap array              */
static size_t   heap_sz = 0;       /* number of elements          */
static size_t   heap_cap = 0;      /* allocated capacity          */

/* Helper: swap two heap nodes */
static inline void heap_swap(HeapNode *a, HeapNode *b)
{
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

/* Restore heap property upwards */
static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

/* Restore heap property downwards */
static void heap_sift_down(size_t i)
{
    for (;;) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < heap_sz && heap[left].val < heap[smallest].val) smallest = left;
        if (right < heap_sz && heap[right].val < heap[smallest].val) smallest = right;

        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* Push a node into the heap */
static void heap_push(const HeapNode node)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 64;
        HeapNode *new_heap = (HeapNode *)malloc(new_cap * sizeof(HeapNode));
        if (!new_heap) abort();
        if (heap) {
            for (size_t k = 0; k < heap_sz; ++k) new_heap[k] = heap[k];
            free(heap);
        }
        heap = new_heap;
        heap_cap = new_cap;
    }
    heap[heap_sz++] = node;
    heap_sift_up(heap_sz - 1);
}

/* Pop the smallest node from the heap */
static HeapNode heap_pop(void)
{
    HeapNode top = heap[0];
    heap[0] = heap[--heap_sz];
    if (heap_sz > 0) heap_sift_down(0);
    return top;
}

/* ----------------------------------------------------------- */
/* Cache of already computed values                            */
static seq_t *cache = NULL;       /* cache[1] .. cache[cache_sz]   */
static size_t cache_sz = 0;       /* number of cached terms        */
static size_t cache_cap = 0;      /* capacity of cache array       */

/* Ensure that the cache contains at least n terms            */
static void ensure_up_to(idx_t n)
{
    if ((size_t)n <= cache_sz) return;

    /* Lazy initialization: fill cache[1] = 1 and build heap          */
    if (cache_sz == 0) {
        /* Allocate initial cache space                                   */
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* a(1) = φ(1) = 1                                               */
        cache[1] = (seq_t)1;
        cache_sz = 1;

        /* ----------------------------------------------------------- */
        /* Generate a list of primes sufficient for the required       */
        /* number of prime powers.  For n up to 10 000, 200 000 is more  */
        /* than enough to provide over 18 000 prime powers.             */
        /* ----------------------------------------------------------- */
        const size_t PRIME_LIMIT = 200000;          /* 2×10^5  */
        char *is_prime = (char *)malloc(PRIME_LIMIT + 1);
        if (!is_prime) abort();
        for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_prime[j] = 0;

        /* Count primes and reserve heap capacity                       */
        size_t prime_cnt = 0;
        for (size_t i = 2; i <= PRIME_LIMIT; ++i) if (is_prime[i]) ++prime_cnt;

        heap_cap = prime_cnt;
        heap_sz  = prime_cnt;
        heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
        if (!heap) abort();

        /* Initialize heap with each prime to the first power (p^1)     */
        size_t idx = 0;
        for (size_t i = 2; i <= PRIME_LIMIT; ++i)
            if (is_prime[i]) {
                heap[idx].prime = i;
                heap[idx].exp   = 1;
                heap[idx].val   = i;      /* p^1 */
                ++idx;
            }

        /* Build the min‑heap                                                    */
        for (size_t i = heap_sz / 2; i > 0; ) { --i; heap_sift_down(i); }

        free(is_prime);
    }

    /* Generate terms until we reach the required index n                    */
    for (size_t next = cache_sz + 1; next <= (size_t)n; ++next) {
        /* Get the next smallest prime power                                 */
        HeapNode node = heap_pop();

        /* Compute φ(p^e) = (p-1)*p^(e-1)                                    */
        uint64_t p = node.prime;
        seq_t power = (seq_t)1;
        for (size_t i = 1; i < node.exp; ++i)
            power = power * (seq_t)p;

        seq_t phi = (seq_t)(p - 1) * power;

        /* Store the new term, enlarge cache if needed                       */
        if (cache_sz + 1 >= cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            for (size_t k = 1; k <= cache_sz; ++k) new_cache[k] = cache[k];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[++cache_sz] = phi;

        /* Push the next power of this prime if it fits in 64‑bit            */
        if (node.val <= UINT64_MAX / node.prime) {
            node.exp++;
            node.val *= node.prime;
            heap_push(node);
        }
    }
}

/* ----------------------------------------------------------- */
/* Public function as required by the framework                 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the OEIS offset   */
    ensure_up_to(n);
    return cache[(size_t)n];
}
