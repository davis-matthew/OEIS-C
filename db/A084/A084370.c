/*
 * A084370  Convolution of odd primes with themselves.
 * https://oeis.org/A084370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A065091(k) * A065091(n-k+1) where A065091(k)
 *            is the k‑th odd prime (3,5,7,11,13,…). The function below builds
 *            the list of odd primes on demand and caches the convolution
 *            values for quick repeated access
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 824, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Macro helpers that work both in C and C++ */
#ifdef __cplusplus
#  define ALLOC_ARRAY(type, n) new type[(n)]
#  define FREE_ARRAY(ptr)      delete[] (ptr)
#else
#  define ALLOC_ARRAY(type, n) ((type*)calloc((n), sizeof(type)))
#  define FREE_ARRAY(ptr)      free(ptr)
#endif

/* static storage for odd primes (excluding 2) */
static seq_t *odd_primes = NULL;      /* dynamic array of primes */
static idx_t   odd_prime_count = 0;   /* number of stored odd primes */

/* static storage for cached a(n) values (1‑based index) */
static seq_t *a_cache = NULL;         /* dynamic array of seq_t values */
static idx_t   a_cache_count = 0;     /* number of cached values */

/* Build or extend the list of odd primes until it contains at least n primes */
static void ensure_primes(idx_t n)
{
    if (odd_prime_count >= n)
        return;                       /* already have enough primes */

    /* Choose a sieve limit that guarantees at least n odd primes.
       Use p_n ~ n log n; a factor of 20 gives a safe margin for all n <= 10⁶. */
    size_t limit = (size_t)n * 20 + 100;
    if (limit < 100) limit = 100;     /* small n safety */

    /* Allocate and initialise sieve array */
    unsigned char *sieve = ALLOC_ARRAY(unsigned char, limit + 1);
    if (!sieve)
        abort();                      /* Out‑of‑memory – terminate */

    for (size_t i = 2; i <= limit; ++i)
        sieve[i] = 1;                  /* assume prime initially */

    /* Classic Eratosthenes sieve */
    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sqrt_limit; ++p) {
        if (sieve[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                sieve[q] = 0;
        }
    }

    /* Extract odd primes (excluding 2) */
    FREE_ARRAY(odd_primes);                 /* discard any previous list */
    odd_prime_count = 0;
    /* Rough upper bound: at most limit/2 odd numbers, plus a bit */
    odd_primes = ALLOC_ARRAY(seq_t, limit / 2 + 10);
    if (!odd_primes)
        abort();                      /* Out‑of‑memory – terminate */

    for (size_t i = 3; i <= limit; i += 2) {
        if (sieve[i]) {
            odd_primes[odd_prime_count++] = (seq_t)i;
        }
    }
    FREE_ARRAY(sieve);
}

/* Extend the cache of a(n) values up to at least n */
static void ensure_cache(idx_t n)
{
    if (a_cache_count >= n)
        return;                       /* already cached */

    /* Allocate a new cache of size n (grow only) */
    seq_t *new_cache = ALLOC_ARRAY(seq_t, n);
    if (!new_cache)
        abort();                      /* Out‑of‑memory – terminate */

    /* Copy existing cached values */
    for (idx_t i = 0; i < a_cache_count; ++i)
        new_cache[i] = a_cache[i];

    /* Free old cache if any */
    FREE_ARRAY(a_cache);
    a_cache = new_cache;

    /* Compute cache entries from a_cache_count+1 to n */
    for (idx_t i = a_cache_count + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += odd_primes[k - 1] * odd_primes[i - k];
        }
        a_cache[i - 1] = sum;
    }

    a_cache_count = n;
}

/* public function: compute a(n) for any n >= 1, independent of call order */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) is unused, but safe to return 0 */

    ensure_primes(n);                  /* make sure we have the first n odd primes */
    ensure_cache(n);                   /* compute and cache the required a(i) values */
    return a_cache[n - 1];
}
