/*
 * A178620  Sum of binary digits ( = sum of ternary digits ) of terms in
 *          A037301.
 * https://oeis.org/A178620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute force search of numbers whose binary digit sum equals ternary
   digit sum.  Returns that common sum for the nth such number
   (zero-based offset).  */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed a(i) values     */
static size_t cache_cap = 0;         /* allocated size for cache         */
static size_t cache_len = 0;         /* number of values currently in cache */
static idx_t candidate = 0;          /* next integer to test             */
static idx_t matched = 0;            /* how many matches have been found */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* abort if allocation fails       */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* extend cache until we have cached up to index n */
    while (matched <= n) {
        /* compute binary digit sum of 'candidate' */
        seq_t bsum = 0;
        idx_t tmp = candidate;
        while (tmp) {
            bsum += tmp & 1;
            tmp >>= 1;
        }

        /* compute ternary digit sum of 'candidate' */
        seq_t tsum = 0;
        tmp = candidate;
        while (tmp) {
            tsum += tmp % 3;
            tmp /= 3;
        }

        if (bsum == tsum) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = bsum;  /* common sum is the same */
            matched++;
        }

        candidate++;
    }

    return cache[n];
}
