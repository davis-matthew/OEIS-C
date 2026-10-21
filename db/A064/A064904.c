/*
 * A064904  Semiprimes p1*p2 such that p2 mod p1 = 6, with p2 > p1.
 * https://oeis.org/A064904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th semiprime p1*p2 with p2 ≡ 6 (mod p1) and p2>p1
 * Range:     u8 none, u16 none, u32 n <= 413, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime64.h"   /* provides bool is_prime64(uint64_t) */

typedef struct {
    seq_t prod;   /* p1 * p2 */
    uint32_t p1;  /* first prime factor */
    seq_t p2;     /* current p2 of this chain */
} Node;

/* --------------------------------------------------------------------- */
/* Prime handling – generate all primes up to LIMIT (2 000 000). */
#define PRIME_LIMIT 2000000
#define MAX_PRIMES 250000         /* enough for PRIME_LIMIT */

static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static void generate_primes(void)
{
    unsigned char *sieve =
        (unsigned char *)calloc(PRIME_LIMIT + 1, 1);   /* cast for C++ */

    for (unsigned i = 2; (unsigned)((long long)i * i) <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (unsigned j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    for (unsigned i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) {
            primes[prime_count++] = i;
        }
    free(sieve);
}

/* --------------------------------------------------------------------- */
/* Simple binary heap of Nodes ordered by prod. */
#define MAX_HEAP MAX_PRIMES
static Node heap[MAX_HEAP];
static size_t heap_size = 0;

static inline int compare_node(const Node *a, const Node *b)
{
    return (a->prod < b->prod) ? -1 : ((a->prod > b->prod) ? 1 : 0);
}

static void heap_push(const Node *val)
{
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (compare_node(&heap[parent], val) <= 0)
            break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = *val;
}

static Node heap_pop(void)
{
    Node min = heap[0];
    Node last = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smaller = (right < heap_size && compare_node(&heap[right], &heap[left]) < 0)
                         ? right : left;
        if (compare_node(&last, &heap[smaller]) <= 0)
            break;
        heap[i] = heap[smaller];
        i = smaller;
    }
    heap[i] = last;
    return min;
}

/* --------------------------------------------------------------------- */
/* Return the next prime of the form cur + k*p1 (k ≥ 1). */
static seq_t next_valid_p2(seq_t cur, uint32_t step)
{
    seq_t cand = cur + step;
    while (!is_prime64((uint64_t)cand)) {
        cand += step;
    }
    return cand;
}

/* Build the initial heap with the first valid p2 for each p1 ≥ 7. */
static void init_heap(void)
{
    if (heap_size > 0) return;   /* already initialized */

    /* Ensure primes are generated */
    if (prime_count == 0) generate_primes();

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p1 = primes[i];
        if (p1 < 7) continue;  /* skip p1 < 7 because smallest such semiprime already > 70? */
        seq_t cand = (seq_t)p1 + 6;  /* first candidate satisfying p2 ≡ 6 (mod p1) */
        while (!is_prime64((uint64_t)cand)) {
            cand += p1;
        }
        Node node = { (seq_t)p1 * cand, p1, cand };
        heap_push(&node);
    }
}

/* --------------------------------------------------------------------- */
/* Cache of already produced terms. */
static seq_t *term_cache = NULL;
static size_t term_count = 0;     /* number of terms stored (term_cache[0] = a(1)) */
static size_t term_capacity = 0;

static void cache_ensure(size_t needed)
{
    if (needed <= term_capacity) return;
    size_t new_cap = term_capacity ? term_capacity << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_capacity = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */

    /* First time: initialise heap and caches. */
    if (!term_cache) {
        init_heap();
    }

    /* Generate terms up to n if needed. */
    while (term_count < (size_t)n) {
        Node cur = heap_pop();

        cache_ensure(term_count + 1);
        term_cache[term_count++] = cur.prod;

        /* Compute the next p2 for the same p1 and push back. */
        seq_t next_p2 = next_valid_p2(cur.p2, cur.p1);
        Node next_node = { (seq_t)cur.p1 * next_p2, cur.p1, next_p2 };
        heap_push(&next_node);
    }

    return term_cache[n - 1];
}
