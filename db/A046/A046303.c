/*
 * A046303  Product of 5 successive primes.
 * https://oeis.org/A046303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=n..n+4} prime(j) (primes are indexed from 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 21, u64 n <= 911, u128 n <= 10000,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* For the OEIS offset, a(1) is the first term.  Calls with n < 1 are not
       specified; return 0 in that case. */
    if (n < 1)
        return 0;

    /* Cached array of the first primes needed so far.
       prime_cache_limit == number of primes stored. */
    static uint32_t *prime_cache = NULL;
    static idx_t prime_cache_limit = 0;

    /* We need primes from index n to n+4 (1‑based), thus a total of
       need = n+4 primes. */
    idx_t need = n + 4;
    if (need > prime_cache_limit) {
        uint32_t *new_arr = NULL;
        size_t  new_cnt = 0;
        /* The helper allocates an array of the first 'need' primes. */
        sieve_first_n_primes(need, &new_arr, &new_cnt);

        /* Replace the old cache.  The helper uses malloc internally;
           free the old memory to avoid leaks. */
        if (prime_cache)
            free(prime_cache);
        prime_cache = new_arr;
        prime_cache_limit = need;
    }

    /* Compute the product of the five successive primes starting at
       prime(n).  Index conversion: prime_cache[0] is prime(1). */
    seq_t prod = 1;
    size_t start = (size_t)(n - 1); /* 0‑based index of prime(n) */
    for (size_t i = start; i < start + 5; ++i) {
        prod *= (seq_t)prime_cache[i];
    }
    return prod;
}
