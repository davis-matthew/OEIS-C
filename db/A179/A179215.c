/*
 * A179215  Product of squarefree numbers less than n+1.
 * https://oeis.org/A179215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n} k ^ A008966(k) (product of all
 *            squarefree numbers ≤ n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* integer square‑root: floor(sqrt(n)) */
static inline idx_t floor_sqrt(idx_t n) {
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1)) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Static storage for memoisation */
    static bool init = false;

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static std::vector<bool> squarefree;
#else
    static seq_t *cache = NULL;
    static bool *squarefree = NULL;
    static size_t cache_len = 0;
#endif

    if (!init) {
#ifdef __cplusplus
        cache.resize(1);
        cache[0] = 1;
        squarefree.resize(1, true);
        squarefree[0] = false;   /* 0 not used */
#else
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
        squarefree = (bool *)malloc(sizeof(bool));
        squarefree[0] = false;
        cache_len = 1;
#endif
        init = true;
    }

    /* Current size of cached values */
#ifdef __cplusplus
    size_t old_size = cache.size();
#else
    size_t old_size = cache_len;
#endif
    idx_t old_limit = (idx_t)old_size - 1U;

    /* Extend cache if necessary */
    if ((idx_t)old_size <= n) {
        idx_t new_n = n;

#ifdef __cplusplus
        cache.resize(new_n + 1);  /* default construct new elements */
        if (squarefree.size() < new_n + 1)
            squarefree.resize(new_n + 1, true);
#else
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_n + 1));
        bool   *new_sq  = (bool   *)realloc(squarefree, sizeof(bool)   * (new_n + 1));
        /* Initialise new entries */
        for (size_t i = old_size; i < (size_t)(new_n + 1); ++i)
            new_sq[i] = true;
        cache = new_cache;
        squarefree = new_sq;
        cache_len = new_n + 1;
#endif

        /* Determine primes up to sqrt(new_n) */
        idx_t maxp = floor_sqrt(new_n);

#ifdef __cplusplus
        std::vector<bool> isPrime(maxp + 1, true);
        if (maxp >= 0) isPrime[0] = false;
        if (maxp >= 1) isPrime[1] = false;
        for (idx_t i = 2; i * i <= maxp; ++i)
            if (isPrime[i])
                for (idx_t j = i * i; j <= maxp; j += i)
                    isPrime[j] = false;
        std::vector<idx_t> primes;
        for (idx_t i = 2; i <= maxp; ++i)
            if (isPrime[i]) primes.push_back(i);
#else
        bool *isPrime = (bool *)malloc(sizeof(bool) * (maxp + 1));
        for (idx_t i = 0; i <= maxp; ++i) isPrime[i] = true;
        if (maxp >= 0) isPrime[0] = false;
        if (maxp >= 1) isPrime[1] = false;
        for (idx_t i = 2; (i * i) <= maxp; ++i)
            if (isPrime[i])
                for (idx_t j = i * i; j <= maxp; j += i)
                    isPrime[j] = false;
        /* collect primes into dynamic array */
        size_t primes_cap = 16, primes_len = 0;
        idx_t *primes_arr = (idx_t *)malloc(sizeof(idx_t) * primes_cap);
        for (idx_t i = 2; i <= maxp; ++i) if (isPrime[i]) {
            if (primes_len >= primes_cap) {
                primes_cap *= 2;
                primes_arr = (idx_t *)realloc(primes_arr, sizeof(idx_t) * primes_cap);
            }
            primes_arr[primes_len++] = i;
        }
#endif

        /* Mark multiples of prime squares greater than old_limit as not squarefree */
#ifdef __cplusplus
        for (idx_t p : primes) {
            idx_t p2 = p * p;
            if (p2 > new_n) break;
            for (idx_t m = p2; m <= new_n; m += p2)
                if (m > old_limit) squarefree[m] = false;
        }
#else
        for (size_t pi = 0; pi < primes_len; ++pi) {
            idx_t p  = primes_arr[pi];
            idx_t p2 = p * p;
            if (p2 > new_n) break;
            for (idx_t m = p2; m <= new_n; m += p2) {
                if (m > old_limit) squarefree[m] = false;
            }
        }
        free(primes_arr);
        free(isPrime);
#endif

        /* Compute products incrementally */
        seq_t prod = cache[old_limit];
        for (idx_t k = old_limit + 1; k <= new_n; ++k) {
            if (squarefree[k]) prod = prod * (seq_t)k;
            cache[k] = prod;
        }
    }

    /* Return cached value */
#ifdef __cplusplus
    return cache[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
