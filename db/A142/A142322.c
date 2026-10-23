/*
 * A142322  Primes congruent to 19 mod 45.
 * https://oeis.org/A142322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 19 modulo 45
 * Range:     u8 n <= 3, u16 n <= 209, u32 n <= 225, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 as a sentinel */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes */
    static seq_t *primes = NULL;
    static idx_t  count  = 0;     /* number of primes cached */
    static size_t cap    = 0;     /* allocated capacity in elements */
    static idx_t  candidate = 19; /* next integer ≡ 19 (mod 45) */

    /* Grow the cache until we have at least n primes */
    while (count < n)
    {
        /* Test the current candidate for primality */
        if (is_prime_big((seq_t)candidate))
        {
            /* Ensure enough space for the new prime */
            if (count == cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                     /* OOM */
                primes = tmp;
                cap    = new_cap;
            }
            /* Store the new prime */
            primes[count++] = (seq_t)candidate;
        }
        /* Advance to the next candidate with residue 19 modulo 45 */
        candidate += 45;
    }

    /* Return the (n-1)-th cached prime */
    return primes[n - 1];
}
