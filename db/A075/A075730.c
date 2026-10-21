/*
 * A075730  Squares of odd semiprimes A046315, odd numbers divisible by
 *          exactly 2 primes (counted with multiplicity).
 * https://oeis.org/A075730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2833, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A075730:
   a(n) = (the nth odd semiprime)², where odd semiprimes are numbers of
   the form p*q with p,q odd primes (p may equal q). */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Data structures                                                     */
/* ------------------------------------------------------------------ */

/* Node in the heap: product value and the indices of the prime pair. */
typedef struct {
    seq_t val;     /* product of two odd primes, to be stored as the square in cache */
    size_t i;      /* index of the first prime (p_i) */
    size_t j;      /* index of the second prime (p_j) */
} heap_node_t;

/* ------------------------------------------------------------------ */
/* Static storage                                                     */
/* ------------------------------------------------------------------ */

static idx_t   cache_len = 0;      /* number of terms cached so far */
static size_t  cache_cap = 0;      /* capacity of the cache array   */
static seq_t * cache      = NULL;  /* cache array                   */

static size_t  prime_cnt  = 0;     /* number of odd primes generated */
static uint64_t *primes   = NULL;  /* array of odd primes           */

static heap_node_t *heap   = NULL; /* min‑heap of prime products     */
static size_t       heap_cap = 0;  /* capacity (current size) of the heap */

/* ------------------------------------------------------------------ */
/* Utility: swap two heap nodes                                       */
/* ------------------------------------------------------------------ */
static void heap_swap(heap_node_t *a, heap_node_t *b)
{
    heap_node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ------------------------------------------------------------------ */
/* Heap percolate down from index 'idx'                               */
/* ------------------------------------------------------------------ */
static void heap_sift_down(size_t idx)
{
    size_t smallest = idx;
    while (1) {
        size_t left  = 2 * idx + 1;
        size_t right = left + 1;

        if (left < heap_cap && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_cap && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;

        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* ------------------------------------------------------------------ */
/* Build the list of odd primes up to PRIME_LIMIT                     */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 200000  /* upper bound for odd primes used */

static void build_primes(void)
{
    if (prime_cnt > 0)         /* already built */
        return;

    /* Sieve of Eratosthenes for odd numbers only */
    char *is_prime = (char *)calloc(PRIME_LIMIT + 1, sizeof(char));
    if (!is_prime) return;  /* allocation failed */

    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = 1;

    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count odd primes */
    size_t cnt = 0;
    for (int i = 3; i <= PRIME_LIMIT; i += 2) {
        if (is_prime[i])
            ++cnt;
    }

    /* Allocate and store odd primes */
    primes     = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    prime_cnt  = cnt;
    size_t idx = 0;
    for (int i = 3; i <= PRIME_LIMIT; i += 2) {
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;
    }

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Initialize the min‑heap of prime products                          */
/* ------------------------------------------------------------------ */
static void init_heap(void)
{
    if (heap_cap > 0)          /* already initialized */
        return;

    build_primes();            /* ensures primes[] and prime_cnt are ready */

    heap_cap = prime_cnt;      /* one node per prime (p_i, p_i) initially   */
    heap = (heap_node_t *)malloc(heap_cap * sizeof(heap_node_t));

    for (size_t k = 0; k < heap_cap; ++k) {
        heap[k].i = k;
        heap[k].j = k;
        heap[k].val = (seq_t)primes[k] * (seq_t)primes[k];
    }

    /* Build min‑heap by sifting down from bottom up */
    for (size_t i = heap_cap / 2; i-- > 0; )
        heap_sift_down(i);
}

/* ------------------------------------------------------------------ */
/* Expand the cache until it contains at least n terms                */
/* ------------------------------------------------------------------ */
static void grow_cache_to(idx_t n)
{
    if (cache_len >= n)
        return;                 /* nothing to do */

    init_heap();                /* ensures heap[0] holds the smallest product */

    /* Ensure cache has enough capacity */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms by extracting from the heap */
    while (cache_len < n) {
        /* Smallest product at heap[0] */
        seq_t prod = heap[0].val;

        /* Store square of the odd semiprime (prod is already product of two primes) */
        cache[cache_len++] = prod * prod;  /* compute square */

        /* Replace root with next product involving the same first prime */
        if (heap[0].j + 1 < prime_cnt) {
            heap[0].j += 1;
            heap[0].val = (seq_t)primes[heap[0].i] * (seq_t)primes[heap[0].j];
            heap_sift_down(0);
        } else {
            /* No more pairs for this i, remove the node by swapping with last and reducing heap size */
            heap_cap -= 1;
            if (heap_cap > 0) {
                heap[0] = heap[heap_cap];   /* move last node to root */
                heap_sift_down(0);
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1 is defined, 0 is undefined */
        return (seq_t)0;

    grow_cache_to(n);
    return cache[n - 1];
}
