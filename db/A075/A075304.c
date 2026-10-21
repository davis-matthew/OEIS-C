/*
 * A075304  Smallest prime greater than the previous term == 1 mod n-th
 *          composite number.
 * https://oeis.org/A075304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p > a(n-1) such that p ≡ 1 (mod c_n),
 *            where c_n is the n‑th composite number (c₁=4)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* memoized a(n), indices start at 1 */
static size_t cache_cap = 0;         /* allocated capacity of cache array */
static size_t cache_size = 0;        /* highest index stored (a(k) for k ≤ size) */

static idx_t comp_candidate = 3;     /* current integer being examined   */
static idx_t comp_counter   = 0;     /* number of composites found so far */

/* Ensure cache can hold index n */
static void ensure_cache_capacity(idx_t n) {
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                     /* undefined for n=0 – safe return */

    /* Lazily initialize cache with a(1) = 5 */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, unlikely */
        cache[0] = 0;                  /* placeholder, unused */
        cache[1] = 5;                  /* a(1) = 5 */
        cache_size = 1;
    }

    ensure_cache_capacity(n);

    /* Return already computed value */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n];

    /* Compute terms up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* Find the k-th composite number */
        while (comp_counter < k) {
            ++comp_candidate;
            if (!is_prime_big((seq_t)comp_candidate))
                ++comp_counter;
        }
        seq_t composite = (seq_t)comp_candidate;   /* c_k */

        seq_t prev = cache[(size_t)k - 1];          /* a(k-1) */

        /* Search for the smallest prime > prev that is ≡ 1 (mod composite) */
        seq_t cand = prev + 1;
        while (!is_prime_big(cand) || (cand % composite != 1))
            ++cand;

        cache[(size_t)k] = cand;
        cache_size = (size_t)k;
    }

    return cache[(size_t)n];
}
