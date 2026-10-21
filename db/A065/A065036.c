/*
 * A065036  Product of the cube of a prime (A030078) and a different prime.
 * https://oeis.org/A065036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065036: product of the cube of a prime and a different prime.
   a(n) = the n-th smallest value of p^3 * q where p and q are distinct primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime generation ---------- */
static uint32_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;     /* number of primes */

static void build_primes(void)
{
    if (primes != NULL)
        return;          /* already built */

    /* Limit large enough to generate 10 000 terms. */
    const uint32_t LIMIT = 200000;
    unsigned char *is_prime = (unsigned char *)calloc(LIMIT + 1, 1);
    if (is_prime == NULL)
        exit(EXIT_FAILURE);

    /* Initialise to prime */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++primes_cnt;

    /* Allocate and fill primes array */
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (primes == NULL)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = i;

    free(is_prime);
}

/* ---------- Min‑heap of candidates ---------- */
struct HeapNode {
    seq_t value;   /* p^3 * q */
    idx_t p_idx;   /* index of p in primes[] */
    idx_t q_idx;   /* index of q in primes[] */
};

static struct HeapNode *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static void heap_init(void)
{
    if (heap != NULL)
        return;          /* already initialised */

    heap_cap = primes_cnt;
    heap = (struct HeapNode *)malloc(heap_cap * sizeof(struct HeapNode));
    if (heap == NULL)
        exit(EXIT_FAILURE);

    heap_size = 0;
    for (idx_t p = 0; p < primes_cnt; ++p) {
        idx_t q = 0;
        if (primes[q] == primes[p])   /* skip equal prime */
            ++q;
        seq_t val = (seq_t)primes[p] * primes[p] * primes[p] * primes[q];
        heap[heap_size++] = (struct HeapNode){val, p, q};
    }

    /* build a min‑heap */
    for (size_t i = heap_size / 2; i > 0; --i)
        /* heapify down from i-1 */
        for (size_t j = i - 1; ; ) {
            size_t left  = (j << 1) + 1;
            size_t right = left + 1;
            if (left >= heap_size)
                break;
            size_t smallest = left;
            if (right < heap_size && heap[right].value < heap[left].value)
                smallest = right;
            if (heap[j].value <= heap[smallest].value)
                break;
            struct HeapNode tmp = heap[j];
            heap[j] = heap[smallest];
            heap[smallest] = tmp;
            j = smallest;
        }
}

/* ---------- Cache of generated sequence values ---------- */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void cache_init(void)
{
    if (seq_cache != NULL)
        return;          /* already init */

    cache_cap = 64;      /* initial capacity */
    seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (seq_cache == NULL)
        exit(EXIT_FAILURE);
    cache_size = 0;
}

/* ---------- Main routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but safe */

    build_primes();
    heap_init();
    cache_init();

    /* Generate values until we have the desired index. */
    while (cache_size < n) {
        /* Grow cache if needed. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                exit(EXIT_FAILURE);
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        /* Pop smallest candidate. */
        struct HeapNode top = heap[0];
        seq_cache[cache_size++] = top.value;

        /* Replace root with last element and heapify down. */
        heap[0] = heap[--heap_size];
        for (size_t i = 0; ; ) {
            size_t left  = (i << 1) + 1;
            size_t right = left + 1;
            if (left >= heap_size)
                break;
            size_t smallest = left;
            if (right < heap_size && heap[right].value < heap[left].value)
                smallest = right;
            if (heap[i].value <= heap[smallest].value)
                break;
            struct HeapNode tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }

        /* Push next candidate for the same p. */
        idx_t p = top.p_idx;
        idx_t q = top.q_idx + 1;
        while (q < primes_cnt && primes[q] == primes[p])
            ++q;
        if (q < primes_cnt) {
            seq_t val = (seq_t)primes[p] * primes[p] * primes[p] * primes[q];
            heap[heap_size++] = (struct HeapNode){val, p, q};
            /* bubble up */
            size_t i = heap_size - 1;
            while (i > 0) {
                size_t parent = (i - 1) >> 1;
                if (heap[parent].value <= heap[i].value)
                    break;
                struct HeapNode tmp = heap[parent];
                heap[parent] = heap[i];
                heap[i] = tmp;
                i = parent;
            }
        }
    }

    return seq_cache[n - 1];
}
