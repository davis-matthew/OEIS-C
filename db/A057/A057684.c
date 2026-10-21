/*
 * A057684  Trajectory of 13 under the '13x+1' map.
 * https://oeis.org/A057684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 13x+1 map: a(0)=13, 
    a(n+1) = a(n)/p   if a(n) is divisible by the smallest prime <13 (p∈{2,3,5,7,11}),
    otherwise a(n+1) = 13*a(n)+1
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* primes smaller than 13 */
static const int SMALL_PRIMES[5] = {2, 3, 5, 7, 11};

/* cache of already computed terms */
static seq_t *CACHE       = NULL;
static idx_t  CACHE_SIZE  = 0;
static idx_t  CACHE_CAP   = 0;

/* extend the cache so that CACHE[n] is valid */
static void ensure_cache(idx_t n)
{
    if (n < CACHE_SIZE)
        return;

    /* initial allocation */
    if (CACHE == NULL) {
        CACHE_CAP = 256;
        CACHE     = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
        CACHE[0]  = (seq_t)13;
        CACHE_SIZE = 1;
    }

    /* grow capacity if necessary */
    if (CACHE_CAP <= n) {
        idx_t new_cap = CACHE_CAP;
        while (new_cap <= n)
            new_cap <<= 1;
        CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        CACHE_CAP = new_cap;
    }

    /* compute terms from CACHE_SIZE up to n */
    for (idx_t i = CACHE_SIZE; i <= n; ++i) {
        seq_t x = CACHE[i - 1];
        seq_t y;
        int  found = 0;

        for (int j = 0; j < 5; ++j) {
            int p = SMALL_PRIMES[j];
            if (x % (seq_t)p == 0) {
                y = x / (seq_t)p;
                found = 1;
                break;
            }
        }

        if (!found)
            y = (seq_t)13 * x + (seq_t)1;

        CACHE[i] = y;
    }

    CACHE_SIZE = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return CACHE[n];
}
