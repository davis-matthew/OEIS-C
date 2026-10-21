/*
 * A096156  Numbers with ordered prime signature (2,1).
 * https://oeis.org/A096156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate the sequence of numbers of the form p^2*q with p<q primes.
   The function a(n) returns the n‑th such number (1‑based).  Values are
   generated on demand and cached.  The implementation uses a min‑heap
   of candidate values and a simple prime sieve.  All allocations cast
   the returned void* to the appropriate type so the code compiles
   under both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>   /* malloc, calloc, realloc */
#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* ---------- Prime sieve ------------------------------------------------ */
static uint64_t *primes = NULL;      /* array of primes */
static size_t primes_cnt = 0;        /* number of primes currently stored */
static uint64_t primes_limit = 0;    /* largest number we have sieved */

static void sieve_to(uint64_t limit)
{
    if (limit <= primes_limit)
        return;

    /* allocate boolean array for sieve */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!is_prime)
        return; /* allocation failure is unlikely */

    is_prime[0] = 0;
    if (limit >= 1)
        is_prime[1] = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++new_cnt;

    /* allocate new primes array */
    uint64_t *new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_primes[pos++] = i;

    /* replace old arrays */
    free(is_prime);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* ---------- Cache for generated terms -------------------------------- */
static seq_t *cache = NULL;          /* array holding the sequence values */
static size_t cache_len = 0;          /* number of terms cached          */
static size_t cache_cap = 0;          /* capacity of the cache           */

static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure is unlikely */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Min‑heap of candidate values ----------------------------- */
struct heap_node {
    seq_t val;     /* p^2 * q */
    size_t p;      /* index of the smaller prime   */
    size_t q;      /* index of the larger prime   */
};

static struct heap_node *heap = NULL;      /* heap array          */
static size_t heap_sz = 0;                 /* current size       */
static size_t heap_cap = 0;                /* current capacity   */

static void grow_heap(size_t min_cap)
{
    if (min_cap <= heap_cap)
        return;
    size_t new_cap = heap_cap ? heap_cap * 2 : 256;
    while (new_cap < min_cap)
        new_cap <<= 1;
    struct heap_node *tmp = (struct heap_node *)realloc(heap, new_cap * sizeof(struct heap_node));
    if (!tmp)
        return;
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(struct heap_node node)
{
    grow_heap(heap_sz + 1);
    heap[heap_sz++] = node;
    /* sift‑up */
    size_t i = heap_sz - 1;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        struct heap_node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static struct heap_node heap_pop(void)
{
    struct heap_node res = heap[0];
    --heap_sz;
    if (heap_sz > 0) {
        heap[0] = heap[heap_sz];
        /* sift‑down */
        size_t i = 0;
        for (;;) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_sz && heap[l].val < heap[smallest].val)
                smallest = l;
            if (r < heap_sz && heap[r].val < heap[smallest].val)
                smallest = r;
            if (smallest == i)
                break;
            struct heap_node tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
    }
    return res;
}

/* ---------- Managing heap entries for new primes --------------------- */
static size_t last_p_added = SIZE_MAX;   /* index of the last p added to heap */

static void add_pending_p_entries(void)
{
    if (primes_cnt < 2)
        return;
    size_t next_p = last_p_added + 1;     /* wrap‑around works for SIZE_MAX */
    while (next_p < primes_cnt - 1) {     /* p must be less than q */
        size_t p = next_p;
        size_t q = p + 1;
        seq_t val = ((seq_t)primes[p]) * primes[p] * primes[q];
        heap_push((struct heap_node){val, p, q});
        last_p_added = p;
        next_p = last_p_added + 1;
    }
}

/* ---------- Generate terms until we have at least 'want' terms -------- */
static void generate_until(size_t want)
{
    if (cache_len >= want)
        return;

    /* at least two primes needed */
    if (primes_cnt < 2)
        sieve_to(1000);   /* quick start with a few primes */

    add_pending_p_entries();

    for (;;) {
        if (cache_len >= want)
            break;

        /* Pop smallest candidate */
        struct heap_node cur = heap_pop();
        grow_cache(cache_len + 1);
        cache[cache_len++] = cur.val;

        /* Push next candidate with same p, next q */
        size_t next_q = cur.q + 1;
        while (next_q >= primes_cnt) {
            /* increase sieve limit exponentially */
            uint64_t new_limit = primes_limit ? primes_limit * 2 : 1000;
            sieve_to(new_limit);
            add_pending_p_entries();   /* new primes may add new p entries   */
        }
        seq_t next_val = ((seq_t)primes[cur.p]) * primes[cur.p] * primes[next_q];
        heap_push((struct heap_node){next_val, cur.p, next_q});
    }
}

/* ---------- Public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* safety; 1‑based indexing in OEIS */
    generate_until((size_t)n);
    return cache[(size_t)n - 1];
}
