/*
 * A172056  Primes p such that 2*p+-1 and 2*p+-3 are all composites.
 * https://oeis.org/A172056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 2p-3, 2p-1, 2p+1, and 2p+3 are
 *            all composite
 * Range:     u8 n <= 4, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static caching to handle out-of-order calls */
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;
    static idx_t last_checked = 1;

    /* Ensure cache is large enough for the requested index */
    while (sz < (size_t)n)
    {
        /* Increment the candidate integer */
        ++last_checked;

        /* Test if last_checked is prime */
        if (!is_prime_big((seq_t)last_checked))
            continue; /* not prime, skip */

        /* Compute 2p +/- {1,3} */
        seq_t p = (seq_t)last_checked;
        seq_t two = (seq_t)2;

        seq_t val1 = two * p - (seq_t)3; /* 2p-3 */
        if (is_prime_big(val1))
            continue; /* composite condition violated */

        seq_t val2 = two * p - (seq_t)1; /* 2p-1 */
        if (is_prime_big(val2))
            continue;

        seq_t val3 = two * p + (seq_t)1; /* 2p+1 */
        if (is_prime_big(val3))
            continue;

        seq_t val4 = two * p + (seq_t)3; /* 2p+3 */
        if (is_prime_big(val4))
            continue;

        /* All four numbers are composite, store p */
        grow_seq(&cache, &cap, sz + 1);
        cache[sz++] = p;
    }

    return cache[n - 1];
}
