/*
 * A118097  Primes of form 2 + odd prime to odd prime power.
 * https://oeis.org/A118097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  AEIS sequence A118097
 *  a(n) = p^q + 2 where p and q are odd primes (p is the base, q is the
 *  exponent).  The sequence is chosen in increasing order of the values,
 *  duplicates are omitted, and only terms that are themselves prime are kept.
 *
 *  The implementation keeps a min‑heap of the next candidate for each base
 *  prime.  As we pop the smallest value we check if it is prime; if so it
 *  goes into the cache.  Then we push the next exponent for the same base
 *  into the heap.  The cache is grown lazily and the function can be
 *  called in any order.
 *
 *  All arithmetic is performed with seq_t, so arbitrary‑precision builds
 *  work unchanged.  Memory allocation results are explicitly cast to
 *  typed pointers to satisfy C++ requirements.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* These helpers are provided by the repository */ 
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

typedef struct {
    seq_t val;          /* p^q + 2 */
    uint32_t base;      /* base prime p */
    size_t exp_idx;     /* index in the primes array (q) */
} Node;

/* ---------- global state ---------- */
static bool init_done = false;
static uint32_t prime_limit = 0;
static uint32_t *primes = NULL;         /* odd primes, primes[0]==3 */
static size_t   prime_count = 0;

static Node *heap = NULL;               /* min‑heap of candidates */
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static seq_t *cache = NULL;             /* cached sequence values */
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* ---------- sieve for odd primes ---------- */
static void generate_primes(uint32_t limit)
{
    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (uint32_t i = 3; (uint64_t)i * i <= limit; i += 2) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= limit; j += 2 * i)
                is_composite[j] = true;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 3; i <= limit; i += 2)
        if (!is_composite[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(1);

    size_t pos = 0;
    for (uint32_t i = 3; i <= limit; i += 2)
        if (!is_composite[i]) primes[pos++] = i;

    prime_count = cnt;
    free(is_composite);
}

/* ---------- min‑heap helper ---------- */
static void heap_push(Node node)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 64;
        Node *new_heap = (Node*)realloc(heap, new_cap * sizeof(Node));
        if (!new_heap) exit(1);
        heap = new_heap;
        heap_cap = new_cap;
    }

    size_t i = heap_size++;
    heap[i] = node;

    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static Node heap_pop(void)
{
    Node root = heap[0];
    --heap_size;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        for (;;) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_size && heap[l].val < heap[smallest].val) smallest = l;
            if (r < heap_size && heap[r].val < heap[smallest].val) smallest = r;
            if (smallest == i) break;
            Node tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
    }
    return root;
}

/* ---------- cache helper ---------- */
static void cache_ensure(size_t need)
{
    if (cache_size + need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    if (cache_size + need > new_cap) new_cap = cache_size + need;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- initialise once ---------- */
static void init_sequence(void)
{
    if (init_done) return;

    /* a generous limit – enough for the first 10 000 terms */
    prime_limit = 2000000;
    generate_primes(prime_limit);

    /* build one node per base prime using the first odd exponent (q=3) */
    heap_cap = prime_count;
    heap = (Node*)malloc(heap_cap * sizeof(Node));
    if (!heap) exit(1);
    heap_size = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        seq_t v = big_pow((seq_t)p, (seq_t)3) + (seq_t)2;
        Node n = { v, p, 0 };          /* exp_idx 0 -> q = 3 */
        heap_push(n);
    }

    cache_size = 0;
    cache_cap  = 0;
    cache = NULL;

    init_done = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* unspecified for n==0 */

    init_sequence();

    /* expand cache until it contains at least n elements */
    while ((idx_t)cache_size < n) {
        if (heap_size == 0) {
            /* should not happen with a large enough prime limit */
            break;
        }

        Node cur = heap_pop();

        /* keep only prime values, avoid duplicates */
        if (is_prime_big(cur.val)) {
            if (cache_size == 0 || cache[cache_size - 1] != cur.val) {
                cache_ensure(1);
                cache[cache_size++] = cur.val;
            }
        }

        /* push the next exponent for this base prime, if any */
        size_t next_idx = cur.exp_idx + 1;
        if (next_idx < prime_count) {
            seq_t next_val = big_pow((seq_t)cur.base,
                                     (seq_t)primes[next_idx]) + (seq_t)2;
            Node new_node = { next_val, cur.base, next_idx };
            heap_push(new_node);
        }
        /* if no more exponents for this base, the node simply disappears */
    }

    return cache[n - 1];
}
