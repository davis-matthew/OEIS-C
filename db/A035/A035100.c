/*
 * A035100  Number of bits in binary expansion of n-th prime.
 * https://oeis.org/A035100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of bits in the binary expansion of the n‑th prime
   (OEIS A035100).  The value is returned as type seq_t.  Primes are
   generated on demand and cached in a static array.  No hard‑coded table
   is used; the algorithm works for any n ≥ 1 and for any order of calls. */
#include <stdlib.h>      /* malloc, realloc, free */
#include <stdbool.h>     /* bool */

 /* Cache of the first primes.  prime_cache[i] holds the (i+1)‑th prime. */
static uint32_t *prime_cache = NULL;
static size_t prime_cache_size = 0;

/* Simple primality test for numbers up to the size of idx_t.
   Uses trial division up to sqrt(x). */
static bool is_prime_candidate(idx_t x)
{
    if (x < 2)          return false;
    if (x == 2)         return true;
    if (x % 2 == 0)     return false;
    for (idx_t d = 3; d <= x / d; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* Ensure prime_cache has at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_size >= (size_t)n)
        return;

    /* Resize the cache to hold n primes. */
    size_t new_cap = (size_t)n;
    uint32_t *tmp = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
    if (!tmp)   /* allocation failed – in practice this will not happen for n ≤ 10000 */
        return;
    prime_cache = tmp;

    /* Find the next primes needed. */
    idx_t candidate;
    if (prime_cache_size == 0) {
        prime_cache[0] = 2;
        prime_cache_size = 1;
        candidate = 3;
    } else {
        candidate = prime_cache[prime_cache_size - 1] + 1;
        if (candidate % 2 == 0)
            candidate++;
    }

    while (prime_cache_size < (size_t)n) {
        while (!is_prime_candidate(candidate))
            candidate += 2;
        prime_cache[prime_cache_size++] = (uint32_t)candidate;
        candidate += 2;   /* next odd candidate */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* offset starts at 1; value for n<1 is arbitrary */
        return (seq_t)0;

    ensure_primes(n);

    /* Compute the binary length of the (n‑th) prime. */
    idx_t prime = (idx_t)prime_cache[n - 1];
    seq_t bits = 0;
    while (prime) {
        prime >>= 1;
        bits++;
    }
    return (seq_t)bits;
}
