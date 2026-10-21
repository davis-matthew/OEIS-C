/*
 * A050936  Sum of two or more consecutive prime numbers.
 * https://oeis.org/A050936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy, changed
 * Formula:   a(n) is the nth smallest distinct sum of two or more consecutive
 *            primes
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds all sums of consecutive prime sequences in ascending
   order using a min‑heap, caches the distinct sums, and returns the requested
   element.  All memory allocations are cast appropriately for C++ compilation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper structure for the min‑heap */
typedef struct {
    seq_t sum;   /* current sum of consecutive primes */
    idx_t start; /* starting index in the primes array (0‑based) */
    idx_t len;   /* number of primes in the sum (≥ 2) */
} heap_node;

/* static state */
static bool          initialized = false;
static uint32_t    *primes      = NULL;    /* array of primes */
static size_t       prime_cnt   = 0;       /* number of primes */

/* prefix sums of primes (prefix[i] = sum of primes[0 .. i-1]) */
static seq_t     *prefix      = NULL;
static size_t     prefix_len  = 0;          /* prime_cnt + 1 */

/* min‑heap of current sums */
static heap_node *heap_arr    = NULL;
static size_t     heap_size   = 0;
static size_t     heap_cap    = 0;

/* cache of distinct sums */
static seq_t     *cache       = NULL;
static idx_t      cache_size  = 0;
static idx_t      cache_cap   = 0;

/* last sum added to cache, to avoid duplicates */
static seq_t      last_sum    = 0;
static bool       has_last_sum = false;

/* min‑heap helpers */
static inline void heap_push(heap_node node) {
    if (heap_size >= heap_cap) {
        heap_cap = (heap_cap == 0) ? 4 : heap_cap * 2;
        heap_arr = (heap_node *)realloc(heap_arr, heap_cap * sizeof(heap_node));
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap_arr[parent].sum <= node.sum)
            break;
        heap_arr[i] = heap_arr[parent];
        i = parent;
    }
    heap_arr[i] = node;
}

static inline heap_node heap_pop(void) {
    heap_node ret = heap_arr[0];
    size_t last = --heap_size;
    if (heap_size == 0)
        return ret;
    heap_node val = heap_arr[last];
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= heap_size)
            break;
        size_t child = l;
        if (r < heap_size && heap_arr[r].sum < heap_arr[l].sum)
            child = r;
        if (val.sum <= heap_arr[child].sum)
            break;
        heap_arr[i] = heap_arr[child];
        i = child;
    }
    heap_arr[i] = val;
    return ret;
}

/* prime generation (simple sieve up to a fixed limit) */
static void gen_primes(void) {
    /* This limit gives more than enough primes for n up to 10000. */
    const uint32_t PRIME_LIMIT = 200000;   /* 17984 primes */
    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = true;

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) {
            primes[idx++] = i;
        }
    prime_cnt = cnt;
    free(is_composite);

    /* Prefix sums */
    prefix_len = prime_cnt + 1;
    prefix     = (seq_t *)malloc(prefix_len * sizeof(seq_t));
    prefix[0] = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i)
        prefix[i + 1] = prefix[i] + (seq_t)primes[i];
}

/* initialization */
static void init(void) {
    if (initialized)
        return;
    gen_primes();

    /* Build initial heap: one node per starting index with len = 2 */
    heap_cap = (prime_cnt > 1) ? (prime_cnt - 1) : 0;
    heap_arr = (heap_node *)malloc(heap_cap * sizeof(heap_node));
    heap_size = 0;
    for (size_t s = 0; s + 1 < prime_cnt; ++s) {
        heap_node n;
        n.start = s;
        n.len   = 2;
        n.sum   = prefix[s + 2] - prefix[s];
        heap_push(n);
    }

    /* Cache of distinct sums */
    cache_cap = 1024;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;

    initialized = true;
}

/* generate terms until reaching target index */
static void ensure_cache(idx_t target) {
    if (cache_size >= target)
        return;

    while (cache_size < target) {
        heap_node cur = heap_pop();

        /* record unique sums */
        if (!has_last_sum || cur.sum != last_sum) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cur.sum;
            last_sum = cur.sum;
            has_last_sum = true;
        }

        /* push the next sum from the same starting index if possible */
        if (cur.start + cur.len < prime_cnt) {
            seq_t new_sum = prefix[cur.start + cur.len + 1] - prefix[cur.start];
            heap_node nxt = { new_sum, cur.start, cur.len + 1 };
            heap_push(nxt);
        }
    }
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for 0, return 0 as safe filler */
    init();
    ensure_cache(n);
    return cache[n - 1];
}
