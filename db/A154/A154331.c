/*
 * A154331  Numbers m such that 12 m^2 is the average of a twin prime pair.
 * https://oeis.org/A154331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 2176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers m such that 12 m² is the average of a twin prime pair,
   i.e. both 12 m²−1 and 12 m²+1 are prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache   = NULL;   /* array of found m values          */
static size_t cache_cap = 0;    /* current capacity of cache        */
static size_t cache_size = 0;   /* number of terms stored so far    */
static idx_t  checked_m = 0;    /* last m examined (starts from 0)  */

/* Ensure the cache can hold at least 'needed' elements.  */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Generate terms until the cache contains at least 'target' terms. */
static void generate_until(size_t target)
{
    while (cache_size < target)
    {
        ++checked_m;                          /* next m to test  */
        seq_t m  = (seq_t)checked_m;          /* cast to seq_t   */
        seq_t v  = m * m;                     /* m²             */
        v *= (seq_t)12;                       /* 12·m²          */

        /* both 12 m²−1 and 12 m²+1 must be prime   */
        if (isPrimeGeneric(v - 1) && isPrimeGeneric(v + 1))
        {
            grow_cache(cache_size + 1);
            cache[cache_size++] = m;         /* store m        */
        }
    }
}

/* Return a(n) with a(1) = first term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined case */
    generate_until((size_t)n);
    return cache[(size_t)(n - 1)];
}
