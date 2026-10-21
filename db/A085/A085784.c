/*
 * A085784  Product of a prime and any number of triangular numbers.
 * https://oeis.org/A085784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   visit_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085784 – Product of a prime and any number of triangular numbers.
 *  a(n) is the n-th smallest integer that can be written as
 *      p · t1 · t2 · … · tr   with p prime,  k(i) ≥ 1 and
 *      ti = k(i)*(k(i)+1)/2 (triangular numbers).
 *  The implementation builds the sequence on demand using a
 *  min‑heap and a hash‑set to avoid duplicates.
 *
 *  The code is standard C11 and will compile with any C++17 compiler
 *  as well.  It relies only on the typedefs seq_t and idx_t which
 *  are supplied by the build system.  No external libraries are
 *  used.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/visit_hash.h"

/* ------------------------------------------------------------------ */
/*  Hash‑set for visited numbers (open addressing, 0 = empty)        */
#define VISIT_INITIAL_CAP (1U << 16)      /* 65 536 entries      */

typedef struct {
    uint64_t *keys;       /* 0 means empty slot                     */
    size_t    size;       /* number of stored elements              */
    size_t    cap;        /* table capacity (power of two)          */
    size_t    mask;       /* cap - 1 (for modulo)                   */
} VisitSet;

static void visit_init(VisitSet *vs) {
    vs->cap   = VISIT_INITIAL_CAP;
    vs->mask  = vs->cap - 1;
    vs->size  = 0;
    vs->keys  = (uint64_t *)calloc(vs->cap, sizeof(uint64_t)); /* all zero */
}

static void visit_resize(VisitSet *vs) {
    size_t new_cap = vs->cap << 1;
    size_t new_mask = new_cap - 1;
    uint64_t *new_keys = (uint64_t *)calloc(new_cap, sizeof(uint64_t));

    for (size_t i = 0; i < vs->cap; ++i) {
        uint64_t key = vs->keys[i];
        if (key == 0) continue;
        size_t idx = visit_hash(key) & new_mask;
        while (new_keys[idx] != 0) idx = (idx + 1) & new_mask;
        new_keys[idx] = key;
    }
    free(vs->keys);
    vs->keys = new_keys;
    vs->cap  = new_cap;
    vs->mask = new_mask;
}

static bool visit_insert(VisitSet *vs, uint64_t key) {
    if (key == 0) return false;            /* 0 is sentinel       */
    size_t idx = visit_hash(key) & vs->mask;
    while (true) {
        uint64_t k = vs->keys[idx];
        if (k == 0) {                      /* empty slot          */
            vs->keys[idx] = key;
            if (++vs->size > vs->cap * 3 / 4) visit_resize(vs);
            return true;                   /* new entry inserted  */
        }
        if (k == key) return false;         /* already present     */
        idx = (idx + 1) & vs->mask;
    }
}

/* ------------------------------------------------------------------ */
/*  Simple binary min‑heap for seq_t numbers                            */
typedef struct {
    seq_t  *data;      /* dynamic array of heap values            */
    size_t  size;      /* current number of elements              */
    size_t  cap;       /* allocated capacity                      */
} MinHeap;

static void heap_init(MinHeap *h) {
    h->cap  = 128;
    h->size = 0;
    h->data = (seq_t *)malloc(h->cap * sizeof(seq_t));
}

static void heap_grow(MinHeap *h) {
    h->cap <<= 1;
    h->data = (seq_t *)realloc(h->data, h->cap * sizeof(seq_t));
}

/* bubble‑up from index i */
static void heap_push(MinHeap *h, seq_t val) {
    if (h->size == h->cap) heap_grow(h);
    size_t i = h->size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= val) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = val;
}

/* pop minimum value */
static seq_t heap_pop(MinHeap *h) {
    seq_t min = h->data[0];
    seq_t last = h->data[--h->size];
    if (h->size == 0) return min;
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t child = (r < h->size && h->data[r] < h->data[l]) ? r : l;
        if (h->data[child] >= last) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return min;
}

/* ------------------------------------------------------------------ */
/*  Main sequence data                                            */
typedef struct {
    seq_t  *terms;   /* cached sequence up to current max index */
    size_t  size;    /* number of cached terms                  */
    size_t  cap;     /* allocated capacity                      */
} Cache;

static void cache_init(Cache *c) {
    c->size = 0;
    c->cap  = 128;
    c->terms = (seq_t *)malloc(c->cap * sizeof(seq_t));
}

static void cache_push(Cache *c, seq_t val) {
    if (c->size == c->cap) {
        c->cap <<= 1;
        c->terms = (seq_t *)realloc(c->terms, c->cap * sizeof(seq_t));
    }
    c->terms[c->size++] = val;
}

/* ------------------------------------------------------------------ */
/*  Primes and triangular numbers (up to a fixed high bound)          */
static seq_t  *prime_list = NULL;
static size_t   prime_cnt  = 0;

static seq_t  *tri_list   = NULL;
static size_t   tri_cnt    = 0;

/* Simple sieve of Eratosthenes – limit 20 000 000 (enough for 10 000 terms) */
#define SIEVE_LIMIT 20000000

static void build_primes(void) {
    bool *is_prime = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    for (size_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    prime_cnt  = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) if (is_prime[i]) ++prime_cnt;
    prime_list = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) if (is_prime[i]) prime_list[idx++] = (seq_t)i;

    free(is_prime);
}

static void build_triangles(void) {
    tri_cnt = 0;
    for (uint64_t k = 1;; ++k) {
        uint64_t t = k * (k + 1) / 2;
        if (t > (uint64_t)SIEVE_LIMIT) break;
        ++tri_cnt;
    }
    tri_list = (seq_t *)malloc(tri_cnt * sizeof(seq_t));
    size_t idx = 0;
    for (uint64_t k = 1;; ++k) {
        uint64_t t = k * (k + 1) / 2;
        if (t > (uint64_t)SIEVE_LIMIT) break;
        tri_list[idx++] = (seq_t)t;
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence object – initialized only once                           */
static bool initialized = false;
static Cache  seq_cache;
static VisitSet visited;
static MinHeap heap;

/* ------------------------------------------------------------------ */
static void init_sequence(void) {
    visit_init(&visited);
    heap_init(&heap);
    cache_init(&seq_cache);

    build_primes();
    build_triangles();

    /* seed heap with all primes and mark them as visited */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        heap_push(&heap, p);
        (void)visit_insert(&visited, (uint64_t)p);
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Generate sequence up to desired index n (1‑based)                  */
static seq_t generate_to(idx_t n) {
    if (!initialized) init_sequence();

    while (seq_cache.size < (size_t)n) {
        /* pop next smallest number from the heap */
        seq_t x = heap_pop(&heap);
        cache_push(&seq_cache, x);

        /* produce new numbers: x * t for each triangular number t */
#if defined(__SIZEOF_INT128__)
        /* use 128‑bit intermediate to avoid overflow at casting */
        unsigned __int128 prod128;
#else
        uint64_t prod64;
#endif
        for (size_t i = 0; i < tri_cnt; ++i) {
            seq_t t = tri_list[i];
#if defined(__SIZEOF_INT128__)
            prod128 = (unsigned __int128)x * (unsigned __int128)t;
            if (prod128 > UINT64_MAX) /* avoid values that can’t be stored in seq_t (small builds) */
                continue;
            seq_t prod = (seq_t)prod128;
#else
            prod64 = (uint64_t)x * (uint64_t)t;
            if (prod64 < (uint64_t)x) /* overflow check */
                continue;
            seq_t prod = (seq_t)prod64;
#endif
            if (visit_insert(&visited, (uint64_t)prod))
                heap_push(&heap, prod);
        }
    }
    return seq_cache.terms[n - 1];
}

/* ------------------------------------------------------------------ */
/*  Public function – exactly the required signature                  */
seq_t a(idx_t n) {
    return generate_to(n);
}
