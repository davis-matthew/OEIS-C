/*
 * A175585  Numbers n with property that sums of neighbor digits of n^2 are
 *          all less than 10.
 * https://oeis.org/A175585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: integers n such that for every pair of adjacent digits of n^2, 
   the sum of the pair is < 10.  The function returns the n‑th element
   of this sequence (a(0)=0). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether a given n satisfies the condition on its square. */
static bool good(idx_t n)
{
    /* Compute n^2 in the type seq_t. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Store the decimal digits of sq in reverse order.  The array is
       large enough for any seq_t that can be handled by the test
       harness (e.g. up to 128 bits => < 40 decimal digits). */
    uint8_t digits[128];
    size_t len = 0;

    /* Handle the zero case separately; otherwise loop while sq > 0. */
    if (sq == 0) {
        digits[len++] = 0;
    } else {
        seq_t cur = sq;
        while (cur != 0) {
            seq_t q = cur / 10;
            seq_t r = cur - q * 10;          /* r is 0..9 */
            digits[len++] = (uint8_t)(unsigned long long)r; /* r fits 0..9 */
            cur = q;
        }
    }

    /* Now check consecutive digit sums in forward order. */
    for (size_t i = 1; i < len; ++i) {
        if ((unsigned)digits[i - 1] + (unsigned)digits[i] >= 10u)
            return false;
    }
    return true;
}

/* Return the n‑th element of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_candidate = 0;

    /* Initialise the cache if it does not exist. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely, but harmless */
    }

    /* Keep generating numbers until we have at least (n+1) terms. */
    while (cache_len <= (size_t)n) {
        if (good(next_candidate)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – stop further growth */
                    break;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[(size_t)n];
}
