/*
 * A227909  Number of ways to write 2*n = p + q with p, q and (p-1)*(q+1) - 1
 *          all prime.
 * https://oeis.org/A227909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 313, u32 n <= 313, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime_in_list, _simple_sieve, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Searches all prime pairs (p,q) with p+q=2*n and checks that
 * (p-1)*(q+1)-1 is prime, counting each ordered pair separately.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* Provides bool is_prime_big(seq_t) */
#include "../utils/_is_prime_in_list.h"
#include "../utils/_simple_sieve.h"

/* Cache of primes up to the largest requested limit. */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_max  = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t limit = 2 * n;                 /* target sum 2*n */

    /* Rebuild the prime cache if needed. */
    if (limit > prime_max) {
        free(prime_cache);
        _simple_sieve(limit, &prime_cache, &prime_cnt);
        prime_max = limit;
    }

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > limit) break;

        uint64_t q = limit - p;
        if (!_is_prime_in_list(q, prime_cache, prime_cnt))
            continue;

        seq_t val = ((seq_t)(p - 1)) * ((seq_t)(q + 1)) - 1;
        if (is_prime_big(val)) ++count;
    }
    return count;
}
