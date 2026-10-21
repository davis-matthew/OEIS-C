/*
 * A059862  a(n) = Product_{i=3..n} (prime(i) - 3).
 * https://oeis.org/A059862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A059862
 *
 * Formula: a(n) = Product_{i=3..n} (prime(i) - 3)
 * with a(1) = a(2) = 1
 * (prime(1)=2, prime(2)=3, prime(3)=5,...)
 *
 * The function is memoised so calls may arrive in any order.
 * It uses a small on‑demand sieve to generate the first n primes.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

/* Helper to grow an array */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        void *tmp = realloc(*ptr, needed * elem_size);
        if (!tmp) abort();             /* allocation failure */
        *ptr = tmp;
        *cap = needed;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_last = 0;     /* highest index already computed */

    static uint32_t *primes = NULL;   /* primes[0] = p1 = 2 */
    static size_t primes_cnt = 0;     /* number of primes stored */

    if (n < 1) return 0;  /* undefined for n<1, not used */

    /* ensure cache has space for indices up to n (1‑based) */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* initialise first two values if not done yet */
    if (cache_last < 1 && n >= 1) cache[1] = 1;
    if (cache_last < 2 && n >= 2) cache[2] = 1;

    /* return stored value if already computed */
    if ((size_t)n <= cache_last) return cache[n];

    /* compute primes if needed */
    if (primes_cnt < (size_t)n) {
        free(primes);          /* discard previous list if any */
        sieve_first_n_primes((idx_t)n, &primes, &primes_cnt);
    }

    /* compute product from cache_last+1 up to n */
    size_t start = 3;
    if (cache_last >= 3) start = cache_last + 1;
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t factor = (seq_t)primes[i - 1] - 3;   /* primes array 0‑based */
        cache[i] = cache[i - 1] * factor;
    }
    cache_last = (size_t)n;
    return cache[n];
}
