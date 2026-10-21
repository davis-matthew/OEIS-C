/*
 * A050221  a(n) = number of sets of consecutive primes whose arithmetic mean
 *          is A060863(n).
 * https://oeis.org/A050221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A122821(A060863(n)) – number of ways to write the nth
 *            mean of consecutive primes as a sum of consecutive primes
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

/* Simple prime sieve up to a specified limit (inclusive). */
static void build_primes(size_t limit, uint32_t **out, size_t *cnt)
{
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) { *out = NULL; *cnt = 0; return; }
    size_t cap = limit / 10 + 1;
    uint32_t *primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t nprimes = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (nprimes == cap) {
                cap <<= 1;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[nprimes++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    *out  = primes;
    *cnt  = nprimes;
}

/* Hash table that maps a 64‑bit key to a 32‑bit count. */
#define HASH_EMPTY 0ULL
#define HASH_LOAD_FACTOR_MULT 70
#define HASH_LOAD_FACTOR_DIV 100

typedef struct {
    size_t cap;
    size_t used;
    uint64_t *keys;
    uint32_t *cnts;
} Uint64CountMap;

static void hash_resize(Uint64CountMap *m)
{
    size_t new_cap = m->cap << 1;
    uint64_t *new_keys = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    uint32_t *new_cnts = (uint32_t *)calloc(new_cap, sizeof(uint32_t));
    for (size_t i = 0; i < m->cap; ++i) {
        if (m->keys[i] != HASH_EMPTY) {
            uint64_t k = m->keys[i];
            size_t h = k & (new_cap - 1);
            while (new_keys[h] != HASH_EMPTY)
                h = (h + 1) & (new_cap - 1);
            new_keys[h] = k;
            new_cnts[h] = m->cnts[i];
        }
    }
    free(m->keys);
    free(m->cnts);
    m->keys = new_keys;
    m->cnts = new_cnts;
    m->cap = new_cap;
}

static Uint64CountMap *hash_new(size_t init_cap)
{
    Uint64CountMap *m = (Uint64CountMap *)malloc(sizeof(Uint64CountMap));
    m->cap = 1;
    while (m->cap < init_cap) m->cap <<= 1;
    m->used = 0;
    m->keys = (uint64_t *)calloc(m->cap, sizeof(uint64_t));
    m->cnts = (uint32_t *)calloc(m->cap, sizeof(uint32_t));
    return m;
}

static void hash_insert(Uint64CountMap *m, uint64_t key)
{
    if (m->used * HASH_LOAD_FACTOR_DIV >= m->cap * HASH_LOAD_FACTOR_MULT)
        hash_resize(m);
    size_t h = key & (m->cap - 1);
    while (m->keys[h] != HASH_EMPTY) {
        if (m->keys[h] == key) {
            ++m->cnts[h];
            return;
        }
        h = (h + 1) & (m->cap - 1);
    }
    m->keys[h] = key;
    m->cnts[h] = 1;
    ++m->used;
}

static void hash_destroy(Uint64CountMap *m)
{
    free(m->keys);
    free(m->cnts);
    free(m);
}

/* Structure for sorting means. */
typedef struct {
    uint64_t mean;
    uint32_t count;
} MeanCount;

/* Global storage for sorted counts. */
static seq_t *sorted_counts = NULL;   /* 1‑based indices via offset 1 */
static idx_t total_terms   = 0;
static bool   initialized  = false;

/* Comparator for qsort on MeanCount by mean. */
static int cmp_mean(const void *a, const void *b)
{
    const MeanCount *x = (const MeanCount *)a;
    const MeanCount *y = (const MeanCount *)b;
    if (x->mean < y->mean) return -1;
    if (x->mean > y->mean) return  1;
    return 0;
}

static void build_sequence(void)
{
    /* 1. Generate a sufficient list of primes. */
    size_t prime_limit = 200000;          /* yields ~17984 primes */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(prime_limit, &primes, &prime_cnt);

    /* 2. Prefix sums of primes (uint64_t). */
    uint64_t *pref = (uint64_t *)malloc((prime_cnt + 1) * sizeof(uint64_t));
    pref[0] = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        pref[i + 1] = pref[i] + (uint64_t)primes[i];

    /* 3. Hash map for mean → count. */
    Uint64CountMap *map = hash_new(1 << 16);

    /* 4. Count all consecutive prime sequences (including single primes). */
    for (size_t s = 0; s < prime_cnt; ++s) {
        uint64_t sum = 0;
        for (size_t e = s; e < prime_cnt; ++e) {
            sum += (uint64_t)primes[e];
            uint64_t len = (uint64_t)(e - s + 1);
            if (sum % len == 0) {
                uint64_t mean = sum / len;
                hash_insert(map, mean);
            }
        }
    }

    /* 5. Convert hash table to an array. */
    size_t sz = map->used;
    MeanCount *arr = (MeanCount *)malloc(sz * sizeof(MeanCount));
    size_t idx = 0;
    for (size_t i = 0; i < map->cap; ++i) {
        if (map->keys[i] != HASH_EMPTY) {
            arr[idx].mean   = map->keys[i];
            arr[idx].count  = map->cnts[i];
            ++idx;
        }
    }

    /* 6. Sort by mean. */
    qsort(arr, sz, sizeof(MeanCount), cmp_mean);

    /* 7. Store sorted counts in an array of seq_t. */
    sorted_counts = seq_alloc(sz);
    for (size_t i = 0; i < sz; ++i)
        sorted_counts[i] = (seq_t)(arr[i].count);
    total_terms = (idx_t)sz;

    /* 8. Clean up. */
    free(arr);
    hash_destroy(map);
    free(pref);
    free(primes);

    initialized = true;
}

/* Public function. */
seq_t a(idx_t n)
{
    if (!initialized)
        build_sequence();

    if (n == 0 || n > total_terms)
        return 0;                /* out of bounds */
    return sorted_counts[n - 1];
}
