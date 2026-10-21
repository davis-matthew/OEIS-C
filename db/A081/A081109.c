/*
 * A081109  9th binomial transform of (1,1,0,0,0,0,0,...).
 * https://oeis.org/A081109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*a(n-1) - 81*a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A081109.
 * The sequence satisfies the linear recurrence
 *   a(n) = 18*a(n-1) - 81*a(n-2)
 * with initial values a(0)=1, a(1)=10.  For any n≥0 the function
 * returns the exact value that fits in seq_t.  If seq_t is a
 * built‑in unsigned integer type the result is computed modulo
 * that type; if seq_t is an arbitrary‑precision class the full
 * integer is produced.  The function is memoised so that calls
 * in any order are efficient. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated array length */
    static idx_t computed  = 0;   /* largest index that has been computed */

    /* initialise on first call */
    if (cache_len == 0) {
        cache_len = 2;                    /* at least indices 0 and 1 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
       
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        computed = 1;
    }

    /* resize cache if required */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;   /* double until large enough */
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)18 * cache[i-1] - (seq_t)81 * cache[i-2];
        }
        computed = n;
    }

    return cache[n];
}
