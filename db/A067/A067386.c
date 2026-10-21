/*
 * A067386  Primes p such that p+1 and p-1 have the same number of distinct
 *          prime factors.
 * https://oeis.org/A067386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1881, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067386: primes p such that ω(p‑1) = ω(p+1) */
/* Computes the nth term by generating primes in ascending order and
 * testing the condition using the helper routines provided by the
 * build system.  The function is fully memoised and works for any
 * order of calls.  Definitions of idx_t, seq_t, and the helper
 * functions are supplied by the build and are not redefined here. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/count_distinct_primes.h"

static seq_t *cache = NULL;   /* array of found terms              */
static size_t cache_cnt = 0;   /* number of terms stored so far     */
static size_t cache_cap = 0;   /* allocated capacity of cache array */
static idx_t next_candidate = 3;  /* next odd integer to test for prime */

/* Ensure that the cache can hold at least 'needed' additional terms.  */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < cache_cnt + needed)
    {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        while (new_cap < cache_cnt + needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                   /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth element of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined index – return 0 for safety */

    /* Expand the cache until the nth term is found.  */
    while (cache_cnt < (size_t)n)
    {
        if (is_prime(next_candidate))
        {
            uint64_t p_minus = (uint64_t)next_candidate - 1;
            uint64_t p_plus  = (uint64_t)next_candidate + 1;
            if (count_distinct_primes(p_minus) == count_distinct_primes(p_plus))
            {
                ensure_cache_capacity(1);
                cache[cache_cnt++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* only test odd numbers */
    }
    return cache[n - 1];
}
