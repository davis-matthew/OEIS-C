/*
 * A189411  Odd primes p such that sigma(p)/2 is a power of an odd prime.
 * https://oeis.org/A189411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 45, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A189411 – odd primes p with σ(p)/2 a power of an odd prime.
   Equivalent to p = 2*q^h – 1 where q is an odd prime and h ≥ 1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/is_prime_big.h"   /* is_prime_big */

typedef struct {
    seq_t  p;      /* candidate prime   */
    uint64_t q;    /* base odd prime    */
    seq_t  power;  /* current power q^h */
} Candidate;

static bool          initialized   = false;
static uint64_t     *odd_primes      = NULL;
static size_t        odd_prime_cnt   = 0;

static Candidate    *heap            = NULL;
static size_t        heap_size       = 0;
static size_t        heap_cap        = 0;

static seq_t        *values          = NULL;
static size_t        values_count    = 0;
static size_t        values_cap      = 0;

/* Swap two candidates in the heap */
static inline void
swap_candidate(Candidate *a, Candidate *b)
{
    Candidate tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Restore heap property downwards from index i */
static void
heapify_down(size_t i)
{
    size_t smallest = i;
    while (1) {
        size_t left  = 2 * i + 1;
        size_t right = left + 1;
        if (left < heap_size && heap[left].p < heap[smallest].p)
            smallest = left;
        if (right < heap_size && heap[right].p < heap[smallest].p)
            smallest = right;
        if (smallest == i)
            break;
        swap_candidate(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* Initialise static data structures on first call */
static void
init_sequence(void)
{
    /* --------- compute odd primes up to a large bound ----------------- */
    const uint64_t LIMIT = 2000000ULL;           /* 2 000 000 is more than enough */
    size_t      primes_cnt = 0;
    idx_t      *primes = NULL;

    build_primes((idx_t)LIMIT, &primes, &primes_cnt);

    /* Count and store only odd primes (skip 2) */
    odd_prime_cnt = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] > 2) odd_prime_cnt++;

    odd_primes = (uint64_t *)malloc(odd_prime_cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] > 2) odd_primes[pos++] = (uint64_t)primes[i];

    free(primes);

    /* --------- initialise heap with the first candidate for each q  ---- */
    heap_cap   = odd_prime_cnt;
    heap_size  = odd_prime_cnt;
    heap       = (Candidate *)malloc(heap_cap * sizeof(Candidate));

    for (size_t i = 0; i < heap_cap; ++i) {
        seq_t pwr = (seq_t)odd_primes[i];
        heap[i].q      = odd_primes[i];
        heap[i].power  = pwr;
        heap[i].p      = (seq_t)2 * pwr - (seq_t)1;
    }

    /* Build min‑heap on p */
    for (int i = (int)heap_size / 2 - 1; i >= 0; --i)
        heapify_down((size_t)i);

    /* --------- values buffer ------------------------------------------ */
    values_cap   = 64;
    values_count = 0;
    values       = (seq_t *)malloc(values_cap * sizeof(seq_t));

    initialized = true;
}

/* Main function required by the judge */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n == 0) return 0;   /* the sequence is 1‑based; safety */

    /* Generate terms up to the requested index */
    while (values_count < (size_t)n) {
        /* Remove the smallest candidate from the heap */
        Candidate top = heap[0];

        /* Prepare next candidate for the same base q */
        seq_t new_pow = top.power * (seq_t)top.q;
        seq_t new_p   = (seq_t)2 * new_pow - (seq_t)1;
        heap[0].power = new_pow;
        heap[0].p     = new_p;
        heapify_down(0);

        /* If the popped candidate is prime, store it */
        if (is_prime_big(top.p)) {
            if (values_count == values_cap) {
                values_cap *= 2;
                values = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
            }
            values[values_count++] = top.p;
        }
    }

    return values[(size_t)n - 1];
}
