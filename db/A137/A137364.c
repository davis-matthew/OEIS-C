/*
 * A137364  Prime numbers n such that n = p1^2 + p2^2 + p3^2, a sum of squares
 *          of 3 distinct prime numbers.
 * https://oeis.org/A137364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 379, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137364: primes of the form 3^2 + p2^2 + p3^2
   with distinct primes p2 < p3, p2,p3 > 3.
   The algorithm generates all such sums in increasing order using
   a min‑heap.  Only the first prime 3 is fixed; the remaining two
   primes are taken from a pre‑computed prime list. */

/* ------------------------------------------------------------------ */
/*  Standard headers                                                    */
/* ------------------------------------------------------------------ */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Integer square root (integer arithmetic)                           */
/* ------------------------------------------------------------------ */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/*  Prime sieve up to a fixed limit (enough for n <= 10000)            */
/* ------------------------------------------------------------------ */
#define PRIME_MAX 1000000U   /* 1e6 is sufficient */

static uint32_t *primes   = NULL;  /* array of primes (uint32_t) */
static size_t   prime_cnt = 0;     /* number of primes */

/* build_primes() must be called at most once */
static void build_primes(void)
{
    size_t  n = PRIME_MAX;
    uint8_t *sieve = (uint8_t*)malloc(n + 1);
    if (!sieve) abort();
    memset(sieve, 1, n + 1);
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= n; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= n; j += i)
                sieve[j] = 0;
        }
    }
    /* count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (sieve[i]) ++prime_cnt;
    /* allocate and fill prime array */
    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;
    free(sieve);
}

/* ------------------------------------------------------------------ */
/*  Test whether a 64‑bit number is prime using the prime list         */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t num)
{
    if (num < 2) return false;
    if (num % 2 == 0) return num == 2;
    uint64_t r = isqrt_u64(num);
    for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i) {
        if (num % primes[i] == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Min‑heap of candidate sums                                        */
/* ------------------------------------------------------------------ */
typedef struct {
    uint64_t sum;   /* 3^2 + p2^2 + p3^2 */
    size_t   p2i;   /* index of p2 in primes array */
    size_t   p3i;   /* index of p3 in primes array */
} Node;

/* push a node onto the heap */
static void heap_push(Node **heap, size_t *size, size_t *cap, Node v)
{
    if (*size == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *heap = (Node*)realloc(*heap, (*cap) * sizeof(Node));
        if (!*heap) abort();
    }
    (*heap)[(*size)++] = v;
    /* bubble up */
    size_t i = *size - 1;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if ((*heap)[parent].sum <= (*heap)[i].sum) break;
        Node tmp = (*heap)[i];
        (*heap)[i] = (*heap)[parent];
        (*heap)[parent] = tmp;
        i = parent;
    }
}

/* pop the smallest node from the heap */
static Node heap_pop(Node **heap, size_t *size)
{
    Node root = (*heap)[0];
    --(*size);
    (*heap)[0] = (*heap)[*size];
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < *size && (*heap)[left].sum < (*heap)[smallest].sum)
            smallest = left;
        if (right < *size && (*heap)[right].sum < (*heap)[smallest].sum)
            smallest = right;
        if (smallest == i) break;
        Node tmp = (*heap)[i];
        (*heap)[i] = (*heap)[smallest];
        (*heap)[smallest] = tmp;
        i = smallest;
    }
    return root;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache and helper data                                    */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;    /* cached sequence values          */
static size_t seq_len  = 0;         /* number of cached entries        */
static size_t seq_cap  = 0;         /* allocated capacity              */

static Node *heap = NULL;           /* min‑heap of sums               */
static size_t heap_size = 0;        /* current heap size              */
static size_t heap_cap  = 0;        /* current heap capacity          */

static size_t p2_start = 0;         /* index of first prime > 3       */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_primes();
        /* index of first prime > 3 (i.e., 5) */
        while (p2_start < prime_cnt && primes[p2_start] <= 3) ++p2_start;
        /* initialize heap with first pair for each viable p2 */
        heap_cap = prime_cnt - p2_start;
        heap = (Node*)malloc(heap_cap * sizeof(Node));
        if (!heap) abort();
        heap_size = 0;
        for (size_t i = p2_start; i + 1 < prime_cnt; ++i) {
            uint64_t p2 = primes[i];
            uint64_t p3 = primes[i + 1];
            uint64_t sum = 9ULL + p2 * p2 + p3 * p3;
            heap_push(&heap, &heap_size, &heap_cap,
                      (Node){sum, i, i + 1});
        }
        initialized = true;
    }

    /* grow cache if necessary */
    if (n > seq_len) {
        if (seq_cap < (size_t)n) {
            seq_cap = (seq_cap == 0) ? 16 : seq_cap;
            while (seq_cap < (size_t)n) seq_cap <<= 1;
            seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            if (!seq_cache) abort();
        }
        while (seq_len < (size_t)n) {
            Node cur = heap_pop(&heap, &heap_size);
            if (is_prime_uint64(cur.sum)) {
                seq_cache[seq_len++] = (seq_t)cur.sum;
            }
            /* advance to next p3 for this p2 */
            ++cur.p3i;
            if (cur.p3i < prime_cnt) {
                uint64_t p2 = primes[cur.p2i];
                uint64_t p3 = primes[cur.p3i];
                uint64_t sum = 9ULL + p2 * p2 + p3 * p3;
                heap_push(&heap, &heap_size, &heap_cap,
                          (Node){sum, cur.p2i, cur.p3i});
            }
            /* if no further combinations for this p2, the node is discarded */
        }
    }
    return seq_cache[(size_t)n - 1];
}
