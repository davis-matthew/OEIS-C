/*
 * A230478  Smallest number divisible by all numbers from 1 to 2*n-1, but not
 *          divisible by n, or 0 if impossible.
 * https://oeis.org/A230478
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 30,
 *            big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto,
 *            count_distinct_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A230478: 
   Smallest positive integer divisible by all numbers from 1 to 2*n-1
   but not divisible by n.  Return 0 when n has more than one distinct
   prime factor.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper utilities from the build system */
#include "../utils/count_distinct_primes.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

/* External function required by the framework */
seq_t a(idx_t n)
{
    /* Values for n < 2 are out of the OEIS offset – return 0 */
    if (n < 2)
        return 0;

    /* If n has more than one distinct prime factor, no such integer exists */
    if (count_distinct_primes((uint64_t)n) != 1)
        return 0;

    /* n is a power of a single prime p */
    uint64_t p = _smallest_prime_factor((uint64_t)n);
    uint64_t tmp = (uint64_t)n;
    int k = 0;                       /* exponent of p in n */
    while (tmp % p == 0) {
        tmp /= p;
        ++k;
    }

    /* limit = 2*n - 1 */
    idx_t limit = 2 * n - 1;
    seq_t limit_seq = (seq_t)limit;

    /* Build or enlarge prime table up to the required limit */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;
    if (prime_list == NULL || limit > prime_limit) {
        /* free old table if necessary */
        if (prime_list != NULL)
            free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    /* Compute the lcm by multiplying prime powers with reduced exponent
       for the prime factor of n itself. */
    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q > limit)
            break;

        /* Greatest exponent e such that q^e <= limit */
        seq_t cur = 1;
        int e = 0;
        while (cur * (seq_t)q <= limit_seq) {
            cur *= (seq_t)q;
            ++e;
        }

        /* If q == p and e equals the exponent of n, drop one power
           because n itself (q^k) is excluded from the set. */
        if (q == p && e == k)
            --e;

        seq_t fac = seq_pow((seq_t)q, (idx_t)e);
        result *= fac;
    }

    return result;
}
