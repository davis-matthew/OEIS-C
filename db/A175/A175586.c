/*
 * A175586  Duplicate of A089995.
 * https://oeis.org/A175586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 33, i16 n <= 39, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth smallest integer that is the product of two distinct
   primes whose indices differ by more than one (i.e. the primes are not
   consecutive). Numbers are sorted in increasing order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* internal data */
static bool initialized = false;
static uint64_t *primes = NULL;     /* list of primes */
static size_t primes_cnt = 0;

/* heap of (value, i, j) where value = primes[i] * primes[j] */
static unsigned long long *heap_vals = NULL;
static size_t *heap_i = NULL;
static size_t *heap_j = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* ---------- helper functions for the binary heap ---------- */
static void heap_heapify_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap_vals[idx] < heap_vals[parent]) {
            /* swap vals */
            unsigned long long tv = heap_vals[idx];
            heap_vals[idx] = heap_vals[parent];
            heap_vals[parent] = tv;
            /* swap i */
            size_t ti = heap_i[idx];
            heap_i[idx] = heap_i[parent];
            heap_i[parent] = ti;
            /* swap j */
            size_t tj = heap_j[idx];
            heap_j[idx] = heap_j[parent];
            heap_j[parent] = tj;
            idx = parent;
        } else {
            break;
        }
    }
}

static void heap_heapify_down(size_t idx)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < heap_size && heap_vals[left] < heap_vals[smallest])
            smallest = left;
        if (right < heap_size && heap_vals[right] < heap_vals[smallest])
            smallest = right;

        if (smallest != idx) {
            /* swap vals */
            unsigned long long tv = heap_vals[idx];
            heap_vals[idx] = heap_vals[smallest];
            heap_vals[smallest] = tv;
            /* swap i */
            size_t ti = heap_i[idx];
            heap_i[idx] = heap_i[smallest];
            heap_i[smallest] = ti;
            /* swap j */
            size_t tj = heap_j[idx];
            heap_j[idx] = heap_j[smallest];
            heap_j[smallest] = tj;
            idx = smallest;
        } else {
            break;
        }
    }
}

static void push_node(unsigned long long val, size_t i, size_t j)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 1;
        heap_vals = (unsigned long long *)realloc(heap_vals,
                                                  heap_cap * sizeof(unsigned long long));
        heap_i    = (size_t *)realloc(heap_i,
                                      heap_cap * sizeof(size_t));
        heap_j    = (size_t *)realloc(heap_j,
                                      heap_cap * sizeof(size_t));
    }
    heap_vals[heap_size] = val;
    heap_i[heap_size]    = i;
    heap_j[heap_size]    = j;
    heap_heapify_up(heap_size);
    ++heap_size;
}

static void pop_node(unsigned long long *out_val,
                     size_t *out_i, size_t *out_j)
{
    *out_val = heap_vals[0];
    *out_i   = heap_i[0];
    *out_j   = heap_j[0];

    --heap_size;
    if (heap_size > 0) {
        heap_vals[0] = heap_vals[heap_size];
        heap_i[0]    = heap_i[heap_size];
        heap_j[0]    = heap_j[heap_size];
        heap_heapify_down(0);
    }
}

/* ---------- prime generation ---------- */
static void generate_primes(void)
{
    /* enough primes to generate 10 000 terms; primes up to 20000 are
       totally sufficient (2262 primes) */
    const size_t LIMIT = 20000;

    uint8_t *is_prime = (uint8_t *)malloc(LIMIT + 1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= LIMIT; k += p)
                is_prime[k] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    primes_cnt = cnt;

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/* ---------- initialization ---------- */
static void init(void)
{
    if (initialized) return;

    generate_primes();

    /* build heap with first non‑consecutive pair for each prime index i */
    heap_cap   = primes_cnt;
    heap_vals  = (unsigned long long *)malloc(heap_cap * sizeof(unsigned long long));
    heap_i     = (size_t *)malloc(heap_cap * sizeof(size_t));
    heap_j     = (size_t *)malloc(heap_cap * sizeof(size_t));
    heap_size  = 0;

    for (size_t ii = 0; ii + 2 < primes_cnt; ++ii) {
        unsigned long long val =
            (unsigned long long)primes[ii] * (unsigned long long)primes[ii + 2];
        push_node(val, ii, ii + 2);
    }

    /* cache for generated terms */
    cache_cap   = 1024;
    seq_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_size  = 0;

    initialized = true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 to satisfy interface */
    if (!initialized) init();

    while ((idx_t)cache_size < n) {
        unsigned long long val;
        size_t i, j;
        pop_node(&val, &i, &j);

        if (cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 1;
            seq_cache = (seq_t *)realloc(seq_cache,
                                         cache_cap * sizeof(seq_t));
        }
        seq_cache[cache_size++] = (seq_t)val;

        size_t next_j = j + 1;
        if (next_j < primes_cnt) {
            unsigned long long new_val =
                (unsigned long long)primes[i] * (unsigned long long)primes[next_j];
            push_node(new_val, i, next_j);
        }
    }

    return seq_cache[n - 1];
}
