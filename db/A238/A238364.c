/*
 * A238364  Numbers n such that 9*n^2+3*n-1 and 9*n^2+3*n+1 are twin primes.
 * https://oeis.org/A238364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 1308, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth integer n for which 
   9*n^2+3*n-1 and 9*n^2+3*n+1 are both prime (twin primes). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* static cache for memoization */
static seq_t *cache = NULL;   /* array of found terms (seq_t) */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t last_checked = 0;/* largest n already tested */

/* ensure cache has at least need capacity */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; a(0) is undefined. */
        return 0;
    }

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        ++last_checked;                      /* candidate n value */
        seq_t m = (seq_t)last_checked;
        seq_t p1 = 9 * m * m + 3 * m - 1;      /* lower twin prime candidate */
        seq_t p2 = p1 + 2;                    /* upper twin prime candidate */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];   /* a(1) corresponds to cache[0] */
}
