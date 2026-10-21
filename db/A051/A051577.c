/*
 * A051577  a(n) = (2*n + 3)!!/3 = A001147(n+2)/3.
 * https://oeis.org/A051577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 3)!! / 3 * Recurrence: a(0)=1, a(n+1) = (2*n + 5)
 *            * a(n) * * This implementation uses only integer arithmetic that
 *            is guaranteed * to be defined for the seq_t type supplied by the
 *            build. It keeps a * static cache so each value is computed at
 *            most once; subsequent * calls return the...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* External type definitions supplied by the build */
extern seq_t a(idx_t n);

/* Cache for previously computed values */
static seq_t *cache = NULL;   /* pointer to the array of values   */
static idx_t  cache_len = 0;  /* number of valid elements in cache */
static idx_t  cache_cap = 0;  /* current allocated capacity       */

seq_t a(idx_t n)
{
    /* Ensure cache has at least one element for a(0) */
    if (cache_len == 0) {
        size_t new_cap = 8; /* initial capacity */
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Handle allocation failure – in the test environment
             * this should never happen. */
            return (seq_t)0;
        }
        cache_cap = new_cap;
        cache[0] = (seq_t)1; /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache until it contains a(n) */
    while (cache_len <= n) {
        /* multiplier for the recurrence: (2*(i-1)+5) where i = cache_len */
        seq_t mul = (seq_t)(2 * (cache_len - 1) + 5);
        seq_t next = cache[cache_len - 1] * mul;

        /* Resize array if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) {
                /* Allocation failed – return zero as a fallback. */
                return (seq_t)0;
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n];
}
