/*
 * A212624  Number of vertices in all independent vertex subsets of the rooted
 *          tree with Matula-Goebel number n.
 * https://oeis.org/A212624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of sizes of all independent vertex subsets of the
 *            rooted * tree with Matula‑Goebel number n. * The implementation
 *            uses a memoised recursion on the * factorisation of n according
 *            to the Matula‑Goebel encoding. * All arithmetic is performed
 *            with the build‑supplied type seq_t
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Limit for the prime sieve: must include all prime factors of n up to the
 * maximum n that will be queried (here 10000, so 200000 is safe). */
#define PRIME_LIMIT 200000

/* Global prime data */
static uint32_t primes[PRIME_LIMIT / 10 + 10];          /* sufficient space for π(200000) ≈ 17984 */
static size_t prime_cnt = 0;
static uint32_t prime_index_map[PRIME_LIMIT + 1] = {0}; /* 1‑based index of prime, 0 if not prime */
static bool primes_ready = false;

/* Initialise the prime list and the index map.  Called once. */
static void init_primes(void)
{
    if (primes_ready) return;

    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) {
            prime_index_map[i] = (uint32_t)(prime_cnt + 1); /* primes are 1‑indexed */
            primes[prime_cnt++] = i;
        }

    free(is_comp);
    primes_ready = true;
}

/* Cache arrays for dynamic programming */
static seq_t *g_ex_cache = NULL;   /* g_ex[n] – number of independent sets where root excluded */
static seq_t *g_incl_cache = NULL; /* g_incl[n] – number of independent sets where root included */
static seq_t *f_ex_cache = NULL;   /* f_ex[n] – sum of sizes over sets where root excluded */
static seq_t *f_incl_cache = NULL; /* f_incl[n] – sum of sizes over sets where root included */
static size_t cache_size = 0;

/* Grow the cache up to at least n.  Elements beyond the old size are
 * initialised to zero. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;

    size_t new_size = n + 1;
    if (new_size < cache_size * 2) new_size = cache_size * 2;

    g_ex_cache   = (seq_t*)realloc(g_ex_cache,   new_size * sizeof(seq_t));
    g_incl_cache = (seq_t*)realloc(g_incl_cache, new_size * sizeof(seq_t));
    f_ex_cache   = (seq_t*)realloc(f_ex_cache,   new_size * sizeof(seq_t));
    f_incl_cache = (seq_t*)realloc(f_incl_cache, new_size * sizeof(seq_t));

    for (size_t i = cache_size; i < new_size; ++i) {
        g_ex_cache[i]   = 0;
        g_incl_cache[i] = 0;
        f_ex_cache[i]   = 0;
        f_incl_cache[i] = 0;
    }
    cache_size = new_size;
}

/* Recursive computation of the four cache values for the tree with
 * Matula‑Goebel number n.  Results are stored in the global cache. */
static void compute(idx_t n)
{
    if (n == 0) return;          /* no tree with number 0; used only for safety */

    if (n < cache_size && g_ex_cache[n] != 0) return; /* already computed */

    if (n == 1) { /* leaf */
        g_ex_cache[1] = 1;   /* empty set */
        f_ex_cache[1] = 0;   /* size 0 */
        g_incl_cache[1] = 1; /* {root} */
        f_incl_cache[1] = 1; /* size 1 */
        return;
    }

    ensure_cache(n);
    /* Factorise n into primes.  Each prime p corresponds to a child with
     * Matula number = index of p in the prime list.   We may have the same
     * prime multiple times (identical subtrees). */
    uint64_t m = n;
    idx_t child_indices[32];
    int child_count = 0;

    for (size_t pi = 0; pi < prime_cnt
                       && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= m; ++pi) {
        uint32_t p = primes[pi];
        while (m % p == 0) {
            child_indices[child_count++] = (idx_t)prime_index_map[p];
            m /= p;
        }
    }
    if (m > 1) { /* remainder is a prime > sqrt(original n) */
        child_indices[child_count++] = (idx_t)prime_index_map[(uint32_t)m];
    }

    /* Compute products and sums for the two cases (root excluded / included). */
    seq_t prod_all = 1;   // product of g_all[child] (here g_ex + g_incl)
    seq_t prod_incl = 1;  // product of g_ex[child]  (for root included)
    for (int i = 0; i < child_count; ++i) {
        idx_t child = child_indices[i];
        compute(child); /* ensures child values are known */

        /* g_all[child] = g_ex[child] + g_incl[child]; */
        seq_t g_all_child = g_ex_cache[child] + g_incl_cache[child];
        prod_all = prod_all * g_all_child;
        prod_incl = prod_incl * g_ex_cache[child];
    }

    /* Sum over all children */
    seq_t sum_all = 0;   // sum of sizes for root excluded
    seq_t sum_incl = 0;  // sum of sizes for root included
    for (int i = 0; i < child_count; ++i) {
        idx_t child = child_indices[i];

        seq_t g_all_child = g_ex_cache[child] + g_incl_cache[child];
        sum_all = sum_all + f_ex_cache[child] * (prod_all / g_all_child)
                     + f_incl_cache[child] * (prod_all / g_all_child);

        sum_incl = sum_incl + f_ex_cache[child] * (prod_incl / g_ex_cache[child]);
    }

    /* The contributions from the root itself (when included) */
    sum_incl = sum_incl + prod_incl;  // root contributes 1 to each set

    g_ex_cache[n]   = prod_all;
    f_ex_cache[n]   = sum_all;
    g_incl_cache[n] = prod_incl;
    f_incl_cache[n] = sum_incl;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_primes();
    ensure_cache(n);          /* make sure caches are large enough */
    compute(n);
    return f_ex_cache[n] + f_incl_cache[n];   /* total sum of sizes */
}
