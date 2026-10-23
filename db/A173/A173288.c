/*
 * A173288  Either n^2+n+{1,9} is prime.
 * https://oeis.org/A173288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173288: a(n) is the nth positive integer n such that
   exactly one of n^2+n+1 or n^2+n+9 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* static cache of already found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t last_test_n = 0;  /* last n that was tested */

/* ensure a buffer of at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap * 2 : 4;
    while (cap < need)
        cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* terminate on allocation failure */
    cache = tmp;
    cache_cap = cap;
}

/* append a value to the cache */
static void append_cache(seq_t val)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        abort(); /* offset is 1, n must be >=1 */

    /* generate terms until we have n of them */
    while ((idx_t)cache_len < n)
    {
        ++last_test_n;                     /* candidate integer m */

        /* use a larger type for the intermediate arithmetic to avoid
           overflow for types like uint8_t */
        uint64_t m = (uint64_t)last_test_n;
        uint64_t p1 = m * m + m + 1;
        uint64_t p2 = m * m + m + 9;

        bool prime1 = is_prime_small(p1);
        bool prime2 = is_prime_small(p2);

        /* include m if exactly one of the values is prime */
        if (prime1 ^ prime2)
            append_cache((seq_t)m);
    }

    return cache[n - 1];
}
