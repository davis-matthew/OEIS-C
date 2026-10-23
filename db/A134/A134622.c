/*
 * A134622  Products pq ("semiprimes") of two primes p and q >= p such that
 *          q+2 is a prime.
 * https://oeis.org/A134622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence consists of the products pq where p and q are
 *            primes with p ≤ q and q+2 is also prime. The terms are listed in
 *            increasing order
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* forward declaration of the required types */
extern seq_t a(idx_t n);

/* ----- static data ----- */
static uint64_t *primes = NULL;          /* all primes ≤ LIMIT */
static size_t primes_sz = 0;

static uint64_t *twin_low = NULL;       /* lower primes in twin pairs */
static size_t twin_sz = 0;

static size_t *max_p_index = NULL;      /* index of largest prime ≤ q for each q */

/* heap nodes: store indices into the twin_low and primes arrays */
struct HeapNode {
    size_t qidx;      /* index into twin_low */
    size_t pidx;      /* index into primes */
    seq_t prod;       /* product primes[pidx] * twin_low[qidx] */
};

static struct HeapNode *heap = NULL; /* min‑heap */
static size_t heap_sz = 0;           /* current number of elements */
static size_t heap_cap = 0;

static seq_t *cache = NULL;      /* memoised outputs of the sequence */
static size_t cache_sz = 0;      /* number of elements cached */
static size_t cache_cap = 0;

/* ---------- heap helpers ---------- */
static void heap_swap(struct HeapNode *a, struct HeapNode *b)
{
    struct HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static void heap_push(struct HeapNode *h, size_t *sz,
                      size_t qidx, size_t pidx, seq_t prod)
{
    if (*sz == heap_cap) {
        heap_cap = (heap_cap == 0) ? 16 : heap_cap * 2;
        heap = (struct HeapNode *)realloc(heap, heap_cap * sizeof(struct HeapNode));
    }
    size_t i = (*sz)++;
    h[i].qidx = qidx;
    h[i].pidx = pidx;
    h[i].prod = prod;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h[parent].prod <= h[i].prod) break;
        heap_swap(&h[parent], &h[i]);
        i = parent;
    }
}

static void heap_pop(struct HeapNode *h, size_t *sz,
                     size_t *qidx, size_t *pidx, seq_t *prod)
{
    *qidx = h[0].qidx;
    *pidx = h[0].pidx;
    *prod = h[0].prod;
    size_t last = --(*sz);
    if (last > 0) {
        h[0] = h[last];
        size_t i = 0;
        while (1) {
            size_t left = (i << 1) + 1;
            if (left > last) break;
            size_t right = left + 1;
            size_t best = left;
            if (right <= last && h[right].prod < h[left].prod) best = right;
            if (h[i].prod <= h[best].prod) break;
            heap_swap(&h[i], &h[best]);
            i = best;
        }
    }
}

/* ---------- prime / twin prime generation ---------- */
static void init_primes_and_twin(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* limit large enough to supply 10 000 terms */
    const idx_t LIMIT = 2000000; /* 2·10⁶ is ample for n ≤ 10 000 */

    /* Sieve of Eratosthenes */
    bool *isP = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= LIMIT; ++i) isP[i] = true;
    isP[0] = isP[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i)
        if (isP[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                isP[j] = false;

    /* count primes to allocate prime array */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (isP[i]) pcnt++;
    primes = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    primes_sz = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (isP[i]) primes[primes_sz++] = i;

    /* collect lower primes of twin pairs */
    twin_low = (uint64_t *)malloc(pcnt * sizeof(uint64_t)); /* at most pcnt */
    twin_sz = 0;
    for (size_t i = 0; i + 1 < primes_sz; ++i)
        if (primes[i + 1] == primes[i] + 2) {
            twin_low[twin_sz++] = primes[i];
        }

    /* allocate array for max prime index per twin prime */
    max_p_index = (size_t *)malloc(twin_sz * sizeof(size_t));
    for (size_t i = 0; i < twin_sz; ++i) {
        uint64_t q = twin_low[i];
        /* binary search in primes array */
        size_t lo = 0, hi = primes_sz;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] <= q) lo = mid + 1;
            else hi = mid;
        }
        max_p_index[i] = (lo == 0) ? 0 : lo - 1;
    }

    /* build initial heap with p = 2 (prime index 0) for each twin prime */
    heap_cap = twin_sz;
    heap_sz = 0;
    if (twin_sz > 0) {
        heap = (struct HeapNode *)malloc(heap_cap * sizeof(struct HeapNode));
        for (size_t i = 0; i < twin_sz; ++i) {
            seq_t prod = (seq_t)primes[0] * (seq_t)twin_low[i];
            heap_push(heap, &heap_sz, i, 0, prod);
        }
    }

    free(isP);
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset < 1, but avoid UB */
    init_primes_and_twin();

    while (cache_sz < (size_t)n) {
        size_t qidx, pidx;
        seq_t prod;
        heap_pop(heap, &heap_sz, &qidx, &pidx, &prod);

        if (cache_sz == cache_cap) {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_sz++] = prod;

        size_t next_p = pidx + 1;
        if (next_p <= max_p_index[qidx]) {
            seq_t new_prod = (seq_t)primes[next_p] * (seq_t)twin_low[qidx];
            heap_push(heap, &heap_sz, qidx, next_p, new_prod);
        }
    }
    return cache[(size_t)n - 1];
}
