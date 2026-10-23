/*
 * A138638  Primes of form 19*n-1.
 * https://oeis.org/A138638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for primes of the form 19*k-1.
   The sequence is indexed starting at 1 as in OEIS. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* array of a(k) values   */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap   = 0;        /* current allocation cap   */
static idx_t last_checked_k = 1;     /* last k that was examined */

/* Ensure capacity for at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;

#ifdef __cplusplus
    /* C++: use new[] and delete[] which invoke constructors/destructors. */
    seq_t *newptr = new seq_t[newcap];
    if (cache)
        delete[] cache;
#else
    /* C: use realloc which only handles POD types. */
    seq_t *newptr = realloc(cache, newcap * sizeof(seq_t));
#endif

    cache     = newptr;
    cache_cap = newcap;
}

/* Return the nth term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)              /* out of range; not used in tests */
        return (seq_t)0;

    /* Grow the cache until the nth term is present. */
    while (cache_size < (size_t)n)
    {
        /* Search for the next prime of the form 19*k-1. */
        for (idx_t k = last_checked_k + 1; ; ++k)
        {
            seq_t candidate = (seq_t)k * 19 - 1;
            if (is_prime_big(candidate))
            {
                ensure_cache(cache_size + 1);
                cache[cache_size++] = candidate;
                last_checked_k = k;
                break;          /* found one, go to next term if needed */
            }
        }
    }

    return cache[n - 1];
}
