/*
 * A192015  Arithmetic derivative of prime powers: a(n) = A003415(A000961(n)).
 * https://oeis.org/A192015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the arithmetic derivative of the n‑th prime‑power, where
 *            the prime powers are sorted by value and the 1st term is 0
 *            (derivative of 1). Equivalently, a(n) = A003415(A000961(n)) =
 *            e·p^(e–1), where the n‑th prime‑power is p^e
 * Range:     u8 n <= 44, u16 n <= 1961, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Global state                                                         */
/* --------------------------------------------------------------------- */
static seq_t *values      = NULL;   /* cache of computed a(n)   */
static size_t values_cap  = 0;       /* allocated capacity       */
static idx_t  max_computed = 0;      /* largest n whose a(n) known */

static uint64_t *primes     = NULL;   /* list of primes          */
static size_t    primes_cnt = 0;      /* number of primes         */

typedef struct {
    __uint128_t val128;   /* prime power (for ordering)          */
    uint64_t    base;     /* prime base                          */
    idx_t       exp;      /* exponent of the base                */
} HeapItem;

static HeapItem *heap     = NULL;
static size_t    heap_size = 0;
static size_t    heap_cap  = 0;

/* --------------------------------------------------------------------- */
/*  Utility functions                                                   */
/* --------------------------------------------------------------------- */
static void ensure_values_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;        /* index 0 unused */
    if (needed <= values_cap) return;
    size_t new_cap = values_cap ? values_cap * 2 : 128;
    while (needed > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    values = tmp;
    values_cap = new_cap;
}

static inline void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val128 <= heap[i].val128) break;
        HeapItem t = heap[p];
        heap[p] = heap[i];
        heap[i] = t;
        i = p;
    }
}

static inline void sift_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t s = i;
        if (l < heap_size && heap[l].val128 < heap[s].val128) s = l;
        if (r < heap_size && heap[r].val128 < heap[s].val128) s = r;
        if (s == i) break;
        HeapItem t = heap[i];
        heap[i] = heap[s];
        heap[s] = t;
        i = s;
    }
}

static inline void heap_push(HeapItem it)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        HeapItem *tmp = (HeapItem *)realloc(heap, heap_cap * sizeof(HeapItem));
        heap = tmp;
    }
    heap[heap_size++] = it;
    sift_up(heap_size - 1);
}

static inline HeapItem heap_pop(void)
{
    HeapItem top = heap[0];
    heap_size--;
    if (heap_size) {
        heap[0] = heap[heap_size];
        sift_down(0);
    }
    return top;
}

/* --------------------------------------------------------------------- */
/*  Prime sieve (simple, up to a modest limit)                           */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 200000
static void sieve_primes(void)
{
    /* Count primes first to allocate array */
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) return;
    size_t count = 0;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = true;
        }
    }
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) count++;

    primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!primes) { free(sieve); return; }

    primes_cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) primes[primes_cnt++] = i;

    free(sieve);
}

/* --------------------------------------------------------------------- */
/*  Initialization                                                      */
/* --------------------------------------------------------------------- */
static void init(void)
{
    sieve_primes();

    /* Build heap of first powers of each prime */
    heap_cap = primes_cnt;
    heap = (HeapItem *)malloc(heap_cap * sizeof(HeapItem));
    for (size_t i = 0; i < primes_cnt; ++i) {
        HeapItem it;
        it.val128 = (__uint128_t)primes[i];
        it.base   = primes[i];
        it.exp    = 1;
        heap[i] = it;
    }
    heap_size = heap_cap;
    for (size_t i = heap_size; i > 0; i >>= 1) sift_down(i - 1);

    /* First term a(1) = 0 (derivative of 1) */
    ensure_values_capacity(1);
    values[1] = (seq_t)0;
    max_computed = 1;
}

/* --------------------------------------------------------------------- */
/*  Compute terms up to target index                                    */
/* --------------------------------------------------------------------- */
static void compute_terms_up_to(idx_t target)
{
    if (!values) init(); /* lazy init */

    if (target <= max_computed) return;
    ensure_values_capacity(target);

    while (max_computed < target) {
        HeapItem cur = heap_pop();

        /* arithmetic derivative: e · p^(e–1)  */
        seq_t val_part = (seq_t)(cur.val128 / cur.base); /* p^(e-1) */
        seq_t deriv = (seq_t)cur.exp * val_part;

        ++max_computed;
        values[max_computed] = deriv;

        /* push next power of the same prime */
        HeapItem nxt;
        nxt.val128 = cur.val128 * (__uint128_t)cur.base;
        nxt.base   = cur.base;
        nxt.exp    = cur.exp + 1;
        heap_push(nxt);
    }
}

/* --------------------------------------------------------------------- */
/*  Public function                                                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    compute_terms_up_to(n);
    return values[n];
}
