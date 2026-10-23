/*
 * A028045  Expansion of 1/((1-3x)(1-4x)(1-8x)(1-11x)).
 * https://oeis.org/A028045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 233*a(n-2) + 844*a(n-3) - 1056*a(n-4) with
 *            a(0)=1, a(1)=26, a(2)=443, a(3)=6304
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache  = NULL;   /* pointer to array of seq_t */
    static idx_t cache_len = 0;    /* number of elements stored (max index + 1) */

    /* if the value is already cached return it immediately */
    if (n < cache_len)
        return cache[n];

    /* need to compute terms up to n */
    idx_t old_len     = cache_len;          /* size before reallocation */
    idx_t new_len     = n + 1;              /* size after reallocation */

    /* reallocate the cache (void* → seq_t* cast needed for C++ compatibility) */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                    /* allocation failed; signal error */
    cache = tmp;
    cache_len = new_len;                    /* updated size */

    /* fill in new entries */
    for (idx_t i = old_len; i < new_len; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)26;
        else if (i == 2)
            cache[2] = (seq_t)443;
        else if (i == 3)
            cache[3] = (seq_t)6304;
        else {
            /* recurrence: a(i) = 26*a(i-1) - 233*a(i-2) + 844*a(i-3) - 1056*a(i-4) */
            seq_t v1 = cache[i-1] * (seq_t)26;
            seq_t v2 = cache[i-2] * (seq_t)233;
            seq_t v3 = cache[i-3] * (seq_t)844;
            seq_t v4 = cache[i-4] * (seq_t)1056;
            cache[i] = v1 - v2 + v3 - v4;
        }
    }

    return cache[n];
}
