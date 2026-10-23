/*
 * A173153  Values of n such that either {0,1}*n^2+n+41 is prime.
 * https://oeis.org/A173153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer m such that exactly one of m+41 or
 *            m^2+m+41 is prime
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

/* Cached sequence values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* current allocation capacity */

/* Ensure the cache has room for at least `need` elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < need)
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but guard against underflow */

    /* Go on generating terms until we have at least n of them. */
    static idx_t next_m = 1;   /* candidate integer to test */

    while (cache_size < n)
    {
        bool ok = false;

        /* test m + 41   (0 * m^2 part) */
        seq_t s1 = (seq_t)next_m + (seq_t)41;
        /* test m^2 + m + 41   (1 * m^2 part) */
        seq_t s2 = (seq_t)next_m * (seq_t)next_m + (seq_t)next_m + (seq_t)41;

        /* Exactly one of them must be prime. */
        ok = isPrimeGeneric(s1) ^ isPrimeGeneric(s2);

        if (ok)
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = (seq_t)next_m;
            cache_size++;
        }
        next_m++;
    }

    return cache[n - 1];
}
