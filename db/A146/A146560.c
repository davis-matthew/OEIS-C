/*
 * A146560  Squarefree semiprimes n such that n+1 is squarefree and n+2 is
 *          prime.
 * https://oeis.org/A146560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 648, u32 n <= 725, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squarefree semiprimes n such that n+1 is squarefree and n+2 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* cache of found terms; cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored so far   */
    static size_t cache_cap  = 0;   /* allocated capacity of cache      */
    static uint64_t next_candidate = 2; /* candidate to test, start at 2  */

    if (n == 0)            /* offset is 1; behaviour below offset doesn't matter */
        return (seq_t)0;

    /* grow the cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* examine the next integer candidate */
        next_candidate++;

        /* test the conditions for the OEIS definition */
        if (is_squarefree((seq_t)next_candidate) &&
            count_distinct_primes(next_candidate) == 2 &&
            is_squarefree((seq_t)(next_candidate + 1)) &&
            is_prime_big((seq_t)(next_candidate + 2)))
        {
            /* enlarge the cache if necessary */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* memory allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
    }

    return cache[n - 1];
}
