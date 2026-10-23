/*
 * A133251  Heptagonal numbers A000566 which are the sum of two other
 *          heptagonal numbers > 0.
 * https://oeis.org/A133251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A133251:
   Heptagonal numbers that can be expressed as the sum of two positive
   heptagonal numbers.
   The routine builds heptagonal numbers up to a dynamic bound,
   gathers all sums that are also heptagonal, removes duplicates,
   sorts them, and caches the result for fast subsequent queries. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ==== Helper hash table for seq_t values ==== */
typedef struct {
    size_t cap, mask, count;
    seq_t *keys;            /* 0 is the empty sentinel (never a real key) */
} HashSet;

static inline size_t hash_seq_t(seq_t x)
{
    /* Simple hash: xor low and high 64‑bit parts (works for 64‑bit too) */
    return (size_t)((uint64_t)x ^ ((uint64_t)(x >> 64)));
}

static void hashset_init(HashSet *hs, size_t expected)
{
    size_t c = 1;
    while (c < expected * 2) c <<= 1;   /* keep load factor <= 0.5 */
    hs->cap   = c;
    hs->mask  = c - 1;
    hs->count = 0;
    hs->keys  = (seq_t *)calloc(c, sizeof(seq_t));
}

static void hashset_free(HashSet *hs)
{
    free(hs->keys);
    hs->keys   = NULL;
    hs->cap    = 0;
    hs->mask   = 0;
    hs->count  = 0;
}

static bool hashset_contains(const HashSet *hs, seq_t key)
{
    if (!key) return false;               /* 0 is unused key */
    size_t h = hash_seq_t(key) & hs->mask;
    while (hs->keys[h]) {
        if (hs->keys[h] == key) return true;
        h = (h + 1) & hs->mask;
    }
    return false;
}

static void hashset_insert(HashSet *hs, seq_t key)
{
    if (!key) return;
    /* Rehash if too full */
    if ((hs->count + 1) * 2 > hs->cap) {
        size_t newcap = hs->cap << 1;
        size_t newmask = newcap - 1;
        seq_t *newkeys = (seq_t *)calloc(newcap, sizeof(seq_t));
        for (size_t i = 0; i < hs->cap; ++i) {
            if (hs->keys[i]) {
                size_t h = hash_seq_t(hs->keys[i]) & newmask;
                while (newkeys[h]) h = (h + 1) & newmask;
                newkeys[h] = hs->keys[i];
            }
        }
        free(hs->keys);
        hs->keys = newkeys;
        hs->cap  = newcap;
        hs->mask = newmask;
    }
    size_t h = hash_seq_t(key) & hs->mask;
    while (hs->keys[h]) {
        if (hs->keys[h] == key) return;
        h = (h + 1) & hs->mask;
    }
    hs->keys[h] = key;
    hs->count++;
}

/* ==== Comparison for qsort on seq_t ==== */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t aa = *(const seq_t *)a;
    seq_t bb = *(const seq_t *)b;
    if (aa < bb) return -1;
    if (aa > bb) return 1;
    return 0;
}

/* ==== Cached data ==== */
static seq_t *hept  = NULL;          /* 1‑based array of heptagonal numbers */
static size_t hept_cap = 0;          /* allocated capacity */

static seq_t *results = NULL;        /* sorted unique sums that are heptagonal */
static size_t results_len = 0;
static size_t results_cap = 0;

static bool cache_done = false;

/* Build heptagonal numbers up to index K (inclusive) */
static void build_hept(idx_t K)
{
    if (hept_cap < K + 1) {
        size_t newcap = K + 1;
        hept = (seq_t *)realloc(hept, newcap * sizeof(seq_t));
        for (idx_t i = hept_cap; i <= K; ++i) {
            seq_t k = i;
            hept[i] = k * ((seq_t)5 * k - 3) / 2;
        }
        hept_cap = newcap;
    }
}

/* Ensure we have at least n terms cached */
static void ensure_cache(idx_t n)
{
    if (cache_done && results_len >= (size_t)n) return;

    idx_t K = n * 2 + 20;   /* starting bound; will grow if needed */
    if (K < 20) K = 20;

    while (1) {
        build_hept(K);

        /* Hash set of all heptagonal numbers up to K */
        HashSet heptSet; hashset_init(&heptSet, K + 10);
        for (idx_t i = 1; i <= K; ++i) hashset_insert(&heptSet, hept[i]);

        /* Prepare result storage */
        results_len = 0;
        if (!results_cap) {
            results_cap = 1024;
            results = (seq_t *)malloc(results_cap * sizeof(seq_t));
        }

        /* Hash set to deduplicate sums */
        HashSet resSet; hashset_init(&resSet, 1024);

        /* Generate all sums of two heptagonal numbers (with i <= j) */
        for (idx_t i = 1; i <= K; ++i) {
            for (idx_t j = i; j <= K; ++j) {
                seq_t sum = hept[i] + hept[j];
                if (hashset_contains(&heptSet, sum)) {
                    if (!hashset_contains(&resSet, sum)) {
                        hashset_insert(&resSet, sum);
                        if (results_len == results_cap) {
                            results_cap <<= 1;
                            results = (seq_t *)realloc(results,
                                                        results_cap * sizeof(seq_t));
                        }
                        results[results_len++] = sum;
                    }
                }
            }
        }

        hashset_free(&heptSet);
        hashset_free(&resSet);

        qsort(results, results_len, sizeof(seq_t), cmp_seq_t);

        if (results_len >= (size_t)n) break;
        /* increase bound and recompute */
        if (K > 40000) K += 10000;   /* safety cap to avoid infinite loop */
        else K <<= 1;
    }
    cache_done = true;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return results[n - 1];
}
