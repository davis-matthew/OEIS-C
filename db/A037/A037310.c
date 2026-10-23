/*
 * A037310  Numbers whose base-3 and base-5 expansions have the same digit
 *          sum.
 * https://oeis.org/A037310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-3 and base-5 digit sums are equal
 * Range:     u8 n <= 71, u16 n <= 9004, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cache_cap   = 0;
    static idx_t   cache_size  = 0;
    static idx_t   next_candidate = 1;  /* start checking from 1 */

    /* Generate terms until we reach the required index n */
    while (cache_size < n)
    {
        seq_t v = (seq_t)next_candidate;
        seq_t sum3 = 0, sum5 = 0;
        seq_t t = v;

        /* digit sum in base 3 */
        while (t)
        {
            sum3 += t % 3;
            t /= 3;
        }

        t = v;
        /* digit sum in base 5 */
        while (t)
        {
            sum5 += t % 5;
            t /= 5;
        }

        /* store if the sums match */
        if (sum3 == sum5)
        {
            if (cache_size == cache_cap)
            {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = v;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
