/*
 * A100951  Number of ways to write n = p*q + r with three distinct primes p,
 *          q and r.
 * https://oeis.org/A100951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = p*q + r * with three distinct
 *            prime numbers p, q, r
 * Range:     u8 n <= 4049, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static data shared across calls */
    static uint8_t  *is_prime_flag = NULL;     /* bitset of primality up to current max */
    static uint64_t *primes          = NULL;   /* array of primes up to current max */
    static size_t   primes_count     = 0;       /* number of primes stored */
    static size_t   primes_cap       = 0;       /* capacity of the primes array */

    static seq_t   *cache   = NULL;             /* memoised results */
    static size_t  cache_sz = 0;                /* size of cache array */
    static idx_t   max_precomputed = 0;         /* largest n for which prime data is valid */

    const seq_t sentinel = ~((seq_t)0);   /* marker for uncomputed cache entries */

    /* Handle trivial small n directly */
    if (n < 8) {
        return 0;
    }

    /*--- enlarge cache if needed -------------------------------------------*/
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) return 0;         /* allocation failure */
        /* initialize new entries to sentinel */
        for (size_t i = cache_sz; i < new_sz; ++i) new_cache[i] = sentinel;
        cache = new_cache;
        cache_sz = new_sz;
    }
    if (cache[n] != sentinel) return cache[n];

    /*--- prime data up to n -----------------------------------------------*/
    if (n > max_precomputed) {
        idx_t limit = n;

        /* allocate and initialise is_prime_flag */
        uint8_t *new_flag = (uint8_t *)malloc((size_t)(limit + 1));
        if (!new_flag) return 0;
        for (idx_t i = 0; i <= limit; ++i) new_flag[i] = 1;
        new_flag[0] = 0;
        new_flag[1] = 0;

        /* Eratosthenes sieve */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (new_flag[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    new_flag[m] = 0;
            }
        }

        /* build primes array, reallocating if needed */
        size_t est_cap = limit / 10 + 10;  /* rough estimate */
        if (primes_cap < est_cap) {
            uint64_t *tmp = (uint64_t *)realloc(primes, est_cap * sizeof(uint64_t));
            if (!tmp) return 0;
            primes = tmp;
            primes_cap = est_cap;
        }

        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (new_flag[i]) {
                if (cnt == primes_cap) {
                    size_t new_cap = primes_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp) return 0;
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[cnt++] = (uint64_t)i;
            }
        }
        primes_count = cnt;

        /* free old flag */
        free(is_prime_flag);
        is_prime_flag = new_flag;
        max_precomputed = limit;
    }

    /*--- count representations ---------------------------------------------*/
    seq_t count = 0;
    idx_t max_prod = n - 2;   /* p*q must be <= max_prod to keep r >= 2 */

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p > max_prod) break;
        for (size_t j = i + 1; j < primes_count; ++j) {
            uint64_t q = primes[j];
            idx_t prod = (idx_t)p * (idx_t)q;
            if (prod > max_prod) break;
            idx_t r = n - prod;
            if (!is_prime_flag[r]) continue;
            if (r == p || r == q) continue;
            count++;
        }
    }

    cache[n] = count;
    return count;
}
