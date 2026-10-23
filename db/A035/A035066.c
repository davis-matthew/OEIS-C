/*
 * A035066  Prime lengths of factorials: see A035065.
 * https://oeis.org/A035066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime that occurs as the number of decimal
 *            digits of n! (duplicate primes allowed). The sequence is
 *            obtained by iterating factorials, counting their decimal digits,
 *            and extracting the primes in order of appearance
 * Range:     u8 n <= 36, u16 n <= 1701, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"

/* Cache of already found prime digit lengths */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* State of the incremental factorial‑digit counter */
static idx_t last_factorial = 0;          /* n whose factorial has been summed */
static long double log_sum = 0.0L;        /* Σ_{i=1}^{last_factorial} log10(i) */

/* Allocate or grow the cache as needed. */
static void
ensure_cache(void)
{
    if (cache == NULL) {
        cache_cap = 128;
        cache_len = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
}

/* Append a new prime digit length to the cache. */
static void
cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* Compute prime digit lengths until the cache has at least 'target'
   entries.  This function incrementally advances the factorial
   calculation. */
static void
compute_until(idx_t target)
{
    if (target == 0) return;          /* offset is 1; n=0 is unused */
    ensure_cache();
    while (cache_len < target) {
        /* Advance factorial to (last_factorial+1)! */
        ++last_factorial;
        log_sum += log10l((long double)last_factorial);

        /* Number of decimal digits of (last_factorial)! */
        unsigned int digits = (unsigned int)(floorl(log_sum) + 1);

        if (is_prime(digits)) {
            cache_append((seq_t)digits);
        }
    }
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at offset 1 */
    compute_until(n);
    return cache[n - 1];
}
