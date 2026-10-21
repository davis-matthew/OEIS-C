/*
 * A076295  Consider all Pythagorean triples (Y-7,Y,Z); sequence gives Y
 *          values.
 * https://oeis.org/A076295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) - 14 (n>=6) initial values: a(0)=4,
 *            a(1)=7, a(2)=12, a(3)=15, a(4)=28, a(5)=55
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External linkage, as required */
seq_t a(idx_t n)
{
    /* Cached sequence values.  Static to persist between calls. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache can hold indices up to n */
    if (n >= cache_size) {
        /* Determine new size: at least n+1, but at least 6 for robustness */
        idx_t new_size = n + 1;
        if (new_size < 6) new_size = 6;

        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure: abort (could also set errno) */
            abort();
        }
        cache = new_cache;

        /* Initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)      cache[i] = (seq_t)4;
            else if (i == 1) cache[i] = (seq_t)7;
            else if (i == 2) cache[i] = (seq_t)12;
            else if (i == 3) cache[i] = (seq_t)15;
            else if (i == 4) cache[i] = (seq_t)28;
            else if (i == 5) cache[i] = (seq_t)55;
            else {
                /* recurrence: a(i) = 6*a(i-3) - a(i-6) - 14 */
                seq_t term = (seq_t)6 * cache[i-3] - cache[i-6] - (seq_t)14;
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
