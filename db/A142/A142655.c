/*
 * A142655  Primes congruent to 33 mod 56.
 * https://oeis.org/A142655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 56 == 33. The function
 *            searches sequentially for primes of the form 56·k+33 and caches
 *            the results
 * Range:     u8 n <= 1, u16 n <= 236, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL; /* buffer of found primes */
static size_t cache_cap  = 0;     /* allocated capacity of prime_cache */
static size_t cache_n    = 0;     /* number of primes stored in prime_cache */

/* Ensure that prime_cache can hold at least 'needed' elements. */
static void enlarge_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory – cannot continue */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Append a newly found prime to the cache. */
static void append_prime(seq_t p)
{
    enlarge_cache(cache_n + 1);
    prime_cache[cache_n++] = p;
}

/* Return the n-th prime ≡ 33 (mod 56).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n == 0 */

    /* 'next_k' is the current k value for the candidate 56·k+33.
       It starts at 1 because 56·0+33 = 33 is not prime. */
    static idx_t next_k = 1;

    /* Generate more primes until we have at least n of them. */
    for ( ; cache_n < n; ++next_k )
    {
        idx_t cand_idx = 33 + next_k * 56; /* candidate fits in idx_t */
        seq_t candidate = (seq_t)cand_idx;

        if (is_prime_big(candidate))
            append_prime(candidate);
    }

    /* Index cache_n-1 corresponds to the n-th prime. */
    return prime_cache[n - 1];
}
