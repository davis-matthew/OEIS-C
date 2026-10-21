/*
 * A201875  Number of zero-sum -n..n arrays of 4 elements with first and
 *          second differences also in -n..n.
 * https://oeis.org/A201875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1)+a(n-2)+a(n-3)-2*a(n-4)-2*a(n-5)+a(n-6)+a(n-7)+a(n-8)-a(n-9)
 * Base values (for 1 <= n <= 9):
 *   a(1)=3, a(2)=19, a(3)=45, a(4)=95, a(5)=173, a(6)=285,
 *   a(7)=431, a(8)=633, a(9)=879
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for 1..9 */
    static const seq_t base[10] = {
        (seq_t)0,         /* unused 0 index */
        (seq_t)3,          /* a(1) */
        (seq_t)19,         /* a(2) */
        (seq_t)45,         /* a(3) */
        (seq_t)95,         /* a(4) */
        (seq_t)173,        /* a(5) */
        (seq_t)285,        /* a(6) */
        (seq_t)431,        /* a(7) */
        (seq_t)633,        /* a(8) */
        (seq_t)879         /* a(9) */
    };

    /* Cache for dynamic computation */
    static seq_t *cache = NULL;           /* array 1..max_computed */
    static idx_t max_computed = 0;        /* largest index computed in cache */
    static idx_t cache_size = 0;          /* allocated size of cache */

    /* Return base if within first 9 terms */
    if (n <= 9) return base[(idx_t)n];

    /* Ensure cache large enough */
    if (cache_size < n) {
        idx_t new_size = (cache_size == 0) ? 16 : cache_size;
        while (new_size < n) new_size <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache) { /* allocation failure -> fall back to base values (shouldn't happen) */
            return base[9];
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Initialize cache with base values up to 9 */
    if (max_computed < 9) {
        for (idx_t i = max_computed + 1; i <= 9; ++i) {
            cache[i] = base[(idx_t)i];
        }
        max_computed = 9;
    }

    /* Compute up to required n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* sum of positive terms */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
        sum += cache[i-6] + cache[i-7] + cache[i-8];
        /* sum of negative terms (note the 2 * a terms) */
        seq_t sub = cache[i-4] + cache[i-4];
        sub += cache[i-5] + cache[i-5];
        sub += cache[i-9];
        /* final value */
        cache[i] = sum - sub;
    }
    max_computed = n;
    return cache[n];
}
