/*
 * A022041  a(n) = 3*a(n-1)+a(n-2)-a(n-3)+a(n-6)-a(n-7)+a(n-10)-a(n-11).
 * https://oeis.org/A022041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A022041
   Recurrence: a(n) = 3*a(n-1)+a(n-2)-a(n-3)+a(n-6)-a(n-7)+a(n-10)-a(n-11)
   Offset: 0 (so a(0) is the first term listed)
   Initial values: 
     a(0)=2, a(1)=6, a(2)=19, a(3)=61, a(4)=196, a(5)=630, a(6)=2026,
     a(7)=6516, a(8)=20957, a(9)=67403, a(10)=216786
*/

#include <stdlib.h>

static const seq_t init[11] = {
    (seq_t)2,    /* a(0) */
    (seq_t)6,    /* a(1) */
    (seq_t)19,   /* a(2) */
    (seq_t)61,   /* a(3) */
    (seq_t)196,  /* a(4) */
    (seq_t)630,  /* a(5) */
    (seq_t)2026, /* a(6) */
    (seq_t)6516, /* a(7) */
    (seq_t)20957,/* a(8) */
    (seq_t)67403,/* a(9) */
    (seq_t)216786/* a(10) */
};

static seq_t *cache = NULL;      /* dynamically expanded cache of sequence terms */
static idx_t cache_len = 0;      /* number of elements currently in cache */

/* Compute a(n) with memoisation – works for any order of calls */
seq_t a(idx_t n)
{
    /* Directly return the known initial values */
    if (n < 11)
        return init[n];

    /* Ensure the cache is large enough to hold values up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 as a harmless placeholder */
            return (seq_t)0;
        cache = tmp;

        /* Initialise the cache with the first 11 terms if not yet done */
        if (cache_len < 11) {
            for (idx_t i = cache_len; i < 11; ++i)
                cache[i] = init[i];
        }

        /* Compute all missing terms up to n */
        for (idx_t i = (cache_len > 11 ? cache_len : 11); i <= n; ++i) {
            cache[i] =
                ( 3 * cache[i-1]
                  +   cache[i-2]
                  -   cache[i-3]
                  +   cache[i-6]
                  -   cache[i-7]
                  +   cache[i-10]
                  -   cache[i-11] );
        }
        cache_len = new_len;
    }

    return cache[n];
}
