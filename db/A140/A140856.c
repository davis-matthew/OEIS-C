/*
 * A140856  Primes of the form 210n+71.
 * https://oeis.org/A140856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the primes of the form 210·k + 71, indexed starting
 *            with a(1) = 71. We find them with a simple incremental search
 *            and cache the results so that any n can be queried in arbitrary
 *            order
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;   /* cached primes */
static idx_t  cache_size  = 0;       /* number of primes stored   */
static idx_t  cache_cap   = 0;       /* capacity of prime_cache   */
static idx_t  next_candidate = 71; /* next value of the form 210k+71 */

/* Ensure the cache can hold at least 'needed' more primes. */
static void ensure_prime_cache_cap(idx_t needed)
{
    if (cache_cap < cache_size + needed) {
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        if (new_cap < cache_size + needed) new_cap = cache_size + needed;
        seq_t *tmp = (seq_t *)realloc(prime_cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, outside the sequence */

    /* Build the cache until it contains at least n primes. */
    while (cache_size < n) {
        if (is_prime(next_candidate)) {
            ensure_prime_cache_cap(1);
            prime_cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 210;
    }

    return prime_cache[n - 1];
}
