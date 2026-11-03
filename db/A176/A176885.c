/*
 * A176885  Let p*q = A006881(n) be the n-th number that is the product of two
 *          distinct primes, with p = prime(i), q=prime(j); a(n) = p^j - q^i.
 * https://oeis.org/A176885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^j - q^i, where p*q = A006881(n) is the n-th * product
 *            of two distinct primes, p = prime(i), q = prime(j), i<j
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* simple prime sieve up to limit (inclusive) */
static void sieve_primes_upto(idx_t limit, uint64_t **prime_arr, size_t *cnt)
{
    if (limit < 2) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_comp) {
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t est = (size_t)(limit / 2 + 10);
    uint64_t *pr = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!pr) {
        free(is_comp);
        *prime_arr = NULL;
        *cnt = 0;
        return;
    }

    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (c < est)
                pr[c++] = (uint64_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = (idx_t)i * (idx_t)i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    *prime_arr = pr;
    *cnt = c;
}

/* ------------------------------------------------------------------ */
/* Data structures                                                      */
/* ------------------------------------------------------------------ */

typedef struct {
    uint64_t prod;   /* p * q  (uint64_t is enough for the primes we use) */
    uint32_t i;      /* index of p in primes array (0‑based) */
    uint32_t j;      /* index of q in primes array (i<j) */
} node_t;

static node_t *heap = NULL;      /* min‑heap of nodes  (products)   */
static size_t heap_size = 0;
static size_t heap_cap   = 0;

static node_t *semis = NULL;     /* growing array of the found semiprimes */
static size_t semi_cnt = 0;
static size_t semi_cap = 0;

/* primes array ----------------------------------------------------- */
static uint64_t *primes = NULL;   /* primes[i] == i‑th prime (0‑based)   */
static size_t primes_cnt = 0;
static const uint64_t PRIME_LIMIT = 200000;   /* large enough for n ≤ 10000 */

/* ------------------------------------------------------------------ */
/* Helper functions for the min‑heap --------------------------------- */
static void heap_init(void) {
    if (heap_cap == 0) {
        heap_cap = 256;
        heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    }
}

static void heap_push(node_t n) {
    heap_init();
    if (heap_size == heap_cap) {
        heap_cap <<= 1;
        heap = (node_t *)realloc(heap, heap_cap * sizeof(node_t));
    }
    size_t k = heap_size++;
    /* bubble up */
    while (k > 0) {
        size_t p = (k - 1) >> 1;
        if (heap[p].prod <= n.prod) break;
        heap[k] = heap[p];
        k = p;
    }
    heap[k] = n;
}

static node_t heap_pop_min(void) {           /* assumes heap_size>0 */
    node_t res = heap[0];
    node_t last = heap[--heap_size];
    size_t k = 0;
    while (true) {
        size_t l = (k << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && heap[r].prod < heap[l].prod) ? r : l;
        if (heap[s].prod >= last.prod) break;
        heap[k] = heap[s];
        k = s;
    }
    heap[k] = last;
    return res;
}

/* ------------------------------------------------------------------ */
/* Initialization routines -------------------------------------------- */
static void init_primes(void) {
    if (primes != NULL) return;          /* already done */
    sieve_primes_upto(PRIME_LIMIT, (uint64_t **)&primes, &primes_cnt);
}

static void init_semis(size_t need) {
    init_primes();
    /* Ensure array to store semis */
    if (semi_cap == 0) {
        semi_cap = need < 8192 ? 8192 : need;
        semis = (node_t *)malloc(semi_cap * sizeof(node_t));
    } else if (semi_cap < need) {
        semi_cap = need;
        semis = (node_t *)realloc(semis, semi_cap * sizeof(node_t));
    }
    /* Build the heap with all pairs (i, i+1)  */
    heap_init();
    heap_size = 0;
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        node_t nd = {primes[i] * primes[i + 1], (uint32_t)i, (uint32_t)(i + 1)};
        heap_push(nd);
    }
    semi_cnt = 0;
}

/* ------------------------------------------------------------------ */
/* ensure_cache ------------------------------------------------------- */
static void ensure_cache(idx_t n) {
    if (n <= semi_cnt) return;         /* already have enough terms     */
    size_t need = (size_t)n;
    if (need > semi_cap) init_semis(need); /* grow storage if needed */
    /* generate until we have n semiprimes */
    while (semi_cnt < need) {
        node_t top = heap_pop_min();
        semis[semi_cnt++] = top;
        if ((size_t)top.j + 1 < primes_cnt) {
            node_t nd = {primes[top.i] * primes[top.j + 1],
                         top.i, (uint32_t)(top.j + 1)};
            heap_push(nd);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function --------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    node_t sp = semis[(size_t)(n - 1)];
    seq_t p = (seq_t)primes[sp.i];
    seq_t q = (seq_t)primes[sp.j];
    idx_t i = (idx_t)(sp.i + 1);
    idx_t j = (idx_t)(sp.j + 1);
    seq_t left  = ipow(p, (seq_t)j);
    seq_t right = ipow(q, (seq_t)i);
    return left - right;
}
