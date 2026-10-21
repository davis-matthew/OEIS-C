/*
 * A088248  Orders of twisted fields.
 * https://oeis.org/A088248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) is the n‑th integer >16 that equals p^k with p prime, k ≥
 *            3, except values of the form 2^q with q prime are omitted. The
 *            code generates all such prime powers in increasing order using a
 *            min‑heap and caches the results
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

/* Min‑heap node holding a prime power value, its base prime and
   current exponent (k ≥ 3). */
typedef struct {
    seq_t  val;          /* current prime power */
    uint64_t prime;      /* base prime      */
    int     exp;         /* current exponent */
} node_t;

/* Swap two heap nodes. */
static void heap_swap(node_t *a, node_t *b) {
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node into the min‑heap. */
static void heap_push(node_t *h, size_t *sz, size_t cap, node_t v) {
    size_t i = *sz;
    if (i >= cap) return;                /* should never happen */
    h[i] = v;
    ++*sz;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].val <= h[i].val) break;
        heap_swap(&h[p], &h[i]);
        i = p;
    }
}

/* Pop the smallest node from the min‑heap. */
static node_t heap_pop(node_t *h, size_t *sz) {
    node_t top = h[0];
    h[0] = h[--*sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= *sz) break;
        size_t child = (r < *sz && h[r].val < h[l].val) ? r : l;
        if (h[i].val <= h[child].val) break;
        heap_swap(&h[i], &h[child]);
        i = child;
    }
    return top;
}

/* Prime sieve (fixed upper bound). */
#define PRIME_LIMIT 200000   /* ample for n ≤ 10000 */

/* Cached prime list. */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;

/* Build a simple sieve of Eratosthenes. */
static void init_primes(void) {
    if (prime_list) return;                 /* already built */
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    sieve[0] = sieve[1] = true;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = true;
        }
    }
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) ++prime_count;
    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    free(sieve);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static state for memoisation and heap construction. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static node_t *heap = NULL;
    static size_t heap_sz = 0, heap_cap = 0;

    /* Initialise primes and heap on first call. */
    if (!heap) {
        init_primes();
        heap_cap = prime_count;
        heap = (node_t *)malloc(heap_cap * sizeof(node_t));
        heap_sz = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            node_t nd;
            nd.prime = prime_list[i];
            nd.exp   = 3;
            nd.val   = pow_uint(nd.prime, (idx_t)nd.exp);
            heap_push(heap, &heap_sz, heap_cap, nd);
        }
    }

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        node_t cur = heap_pop(heap, &heap_sz);

        /* Test inclusion condition. */
        if (cur.val > (seq_t)16 &&
            !(cur.prime == 2 && is_prime((idx_t)cur.exp))) {
            /* Append to cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur.val;
        }

        /* Push the next power of the same prime onto the heap. */
        int next_exp = cur.exp + 1;
        seq_t next_val = pow_uint(cur.prime, (idx_t)next_exp);
        /* For fixed‑width types guard against wrap‑around; unneeded for bigint. */
        if (next_val > cur.val) {
            node_t nxt;
            nxt.prime = cur.prime;
            nxt.exp   = next_exp;
            nxt.val   = next_val;
            heap_push(heap, &heap_sz, heap_cap, nxt);
        }
    }

    return cache[n - 1];
}
