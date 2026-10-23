/*
 * A138456  a(n) = ((n-th prime)^6-(n-th prime)^4)/6.
 * https://oeis.org/A138456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-th prime)^6 - (n-th prime)^4)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* offset handling – calls below 1 do not matter, return 0 */
    if (n == 0)
        return 0;

    /* static cache of primes up to the highest n requested so far */
    static idx_t *prime_cache = NULL;
    static size_t prime_cache_n = 0; /* number of primes cached */

    /* enlarge cache if necessary */
    if (n > prime_cache_n)
    {
        idx_t limit = (idx_t)_prime_limit_for_idx(n);
        idx_t *new_primes   = NULL;
        size_t new_count    = 0;

        /* build_primes fills the array and sets the count */
        build_primes(limit, &new_primes, &new_count);

        /* free old cache and replace */
        free(prime_cache);
        prime_cache   = new_primes;
        prime_cache_n = new_count;
    }

    /* retrieve the n-th prime (prime_cache is 0-indexed) */
    idx_t p = prime_cache[n - 1];

    /* compute ((p^6 - p^4) / 6) using seq_t arithmetic */
    seq_t ps  = (seq_t)p;
    seq_t p2  = ps * ps;          /* p^2 */
    seq_t p4  = p2 * p2;          /* p^4 */
    seq_t num = p4 * (p2 - 1);    /* p^4 * (p^2 - 1) == p^6 - p^4 */
    seq_t res = num / 6;          

    return res;
}
