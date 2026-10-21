/*
 * A076471  Number of pairs (p,q) of successive primes with p+q<=n.
 * https://oeis.org/A076471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts the successive prime pairs (p,q) with p+q <= n
 * Range:     u8 n <= 3239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to the greatest n requested so far */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    /* Re-build the prime list if a larger n is requested */
    if (n > prime_limit) {
        if (primes != NULL)
            free(primes);                                   /* free old list */
        build_primes(n, &primes, &prime_cnt);                /* primes <= n */
        prime_limit = n;
    }

    seq_t count = 0; /* result type seq_t */
    /* Count successive pairs whose sum does not exceed n */
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        if (primes[i] + primes[i + 1] <= n)
            ++count;
        else
            break; /* sums only increase */
    }
    return count;
}
