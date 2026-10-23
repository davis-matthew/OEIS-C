/*
 * A115827  Integers i such that 5*i XOR 6*i = 11*i.
 * https://oeis.org/A115827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115827: Integers i such that 5*i XOR 6*i = 11*i
 *
 * This implementation tests the equivalent condition
 * (5*i) & (6*i) == 0.  The sequence is generated on demand and
 * memoised in a static dynamic array so that subsequent calls
 * are answered in O(1) after the required elements have been
 * generated.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding all found terms up to the current maximum n */
    static seq_t *cache = NULL;
    static size_t cache_len     = 0;
    static size_t cache_cap     = 0;
    static idx_t next_i_to_test = 0;          /* next i to evaluate */

    /* extend cache until we have at least n+1 terms */
    while (cache_len <= n)
    {
        seq_t val5 = (seq_t)next_i_to_test * (seq_t)5;
        seq_t val6 = (seq_t)next_i_to_test * (seq_t)6;

        if ((val5 & val6) == (seq_t)0)          /* condition satisfied */
        {
            /* grow array if needed */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_i_to_test;
        }
        next_i_to_test++;                       /* move to next i */
    }

    return cache[n];
}
