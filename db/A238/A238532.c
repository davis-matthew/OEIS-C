/*
 * A238532  Number of distinct factorial numbers congruent to -1 (mod n).
 * https://oeis.org/A238532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of k ≥ 1 such that k! ≡ −1 (mod n)
 * Range:     u8 n <= 4612, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for previously computed values of a(n) */
static seq_t *cache = NULL;          /* computed a(n) values      */
static uint8_t *cached = NULL;       /* 1 if cache[n] is valid    */
static size_t cache_size = 0;        /* current allocation size   */

/* Static array of smallest prime factors for integers up to the
   largest n that has been requested. */
static uint64_t *spf = NULL;         /* spf[x] = smallest prime factor of x, x ≥ 2  */
static size_t spf_size = 0;          /* number of entries in spf  */

/* Ensure that the cache can hold indices up to n.  Grow lazily. */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size) return;
    size_t new_size = n + 1;
    if (new_size < cache_size * 2) new_size = (cache_size ? cache_size * 2 : 16);

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    uint8_t *new_cached = (uint8_t *)realloc(cached, new_size * sizeof(uint8_t));
    if (!new_cached) exit(EXIT_FAILURE);

    for (size_t i = cache_size; i < new_size; ++i) {
        new_cache[i] = 0;
        new_cached[i] = 0;
    }
    cache = new_cache;
    cached = new_cached;
    cache_size = new_size;
}

/* Ensure that the smallest‑prime‑factor table is available up to limit. */
static void ensure_spf(idx_t limit)
{
    if ((idx_t)spf_size > limit) return;
    size_t new_size = limit + 1;
    uint64_t *new_spf = (uint64_t *)realloc(spf, new_size * sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    for (size_t i = spf_size; i < new_size; ++i) new_spf[i] = 0;
    spf = new_spf;

    /* Sieve to fill spf entries. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {                  /* i is prime */
            spf[i] = (uint64_t)i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint64_t)i;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint64_t)i;
    spf_size = new_size;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 for safety */

    ensure_cache_size(n);
    if (cached[n]) return cache[n];

    /* Build spf table up to n for composite detection. */
    ensure_spf(n);

    /* Count solutions k: k! ≡ n‑1 (mod n). */
    seq_t count = 0;
    seq_t mod_n = (seq_t)n;
    seq_t target = mod_n - (seq_t)1;       /* n‑1 modulo n */

    /* For composite n, exceed of spf[n] guarantees the factorial is
       divisible by a prime factor of n, hence cannot be ≡ −1 mod n. */
    idx_t k_lim = n - 1;
    if (n > 1 && spf[n] < (uint64_t)n)   /* composite */
        k_lim = (idx_t)(spf[n] - 1);

    seq_t fact = 1;
    for (idx_t k = 1; k <= k_lim; ++k) {
        fact = (fact * (seq_t)k) % mod_n;
        if (fact == target) ++count;
    }

    cache[n] = count;
    cached[n] = 1;
    return count;
}
