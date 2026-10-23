/*
 * A117929  Number of partitions of n into 2 distinct primes.
 * https://oeis.org/A117929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered pairs of distinct primes whose sum is
 *            n
 * Range:     u8 n <= 6929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static idx_t _primes_max = 0;
static idx_t *_primes     = NULL;
static size_t _primes_cnt = 0;

seq_t a(idx_t n)
{
    /* Grow our prime table if necessary */
    if (n > _primes_max) {
        free(_primes);
        build_primes(n, &_primes, &_primes_cnt);
        _primes_max = n;
    }

    seq_t count = 0;
    /* Iterate over primes not exceeding n/2 */
    for (size_t i = 0; i < _primes_cnt; ++i) {
        idx_t p = _primes[i];
        if (p > n / 2) break;
        idx_t q = n - p;
        if (q != p && is_prime(q))
            ++count;
    }
    return count;
}
