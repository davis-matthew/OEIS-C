/*
 * A023711  Numbers with exactly 2 1's in their base 4 expansion.
 * https://oeis.org/A023711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers with exactly two 1's in their base-4 expansion
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour for n==0 is irrelevant */

    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_start = 0;      /* next number to examine */

    /* allocate or enlarge cache when needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < (size_t)n) {
        seq_t x = next_start;
        /* count the number of digit 1's in the base-4 expansion of x */
        seq_t t = x;
        int ones = 0;
        while (t > 0) {
            if ((t % 4) == 1) {
                ++ones;
                if (ones > 2) break; /* early exit if more than two 1's */
            }
            t /= 4;
        }
        if (ones == 2) {           /* valid term */
            if (cache_len >= cache_cap) {
                /* double the capacity */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = x;
        }
        next_start = x + 1;
    }

    return cache[n - 1];
}
