/*
 * A059046  Numbers n such that sigma(n)-n divides n-1.
 * https://oeis.org/A059046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that (sigma(m)-m) divides (m-1)
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here sigma(m) is the sum of all positive divisors of m. */
/* Additional requirement: The sequence includes primes and prime powers,
   but also other integers that satisfy the divisibility condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"   /* provides seq_t sigma_of(idx_t n) */

/* Test whether a given integer m satisfies the defining property. */
static bool _qualifies(uint64_t m)
{
    seq_t sigma = sigma_of(m);                 /* sum of divisors of m   */
    seq_t diff   = sigma - (seq_t)m;            /* sigma(m)-m             */
    if (diff == 0)             /* m = 1 would give zero divisor; skip it */
        return false;
    return (((seq_t)(m - 1)) % diff) == 0;      /* (m-1) divisible by diff */
}

/* Return the nth element of A059046.  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache of generated terms.  The cache is grown
       as needed; it stores up to 'cache_cap' elements. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of terms stored          */
    static size_t cache_cap  = 0;   /* allocated capacity              */
    static uint64_t last_checked = 1;  /* last integer examined         */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n)
    {
        /* Grow the cache if necessary. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next integer that satisfies the property. */
        ++last_checked;
        if (_qualifies(last_checked))
        {
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
