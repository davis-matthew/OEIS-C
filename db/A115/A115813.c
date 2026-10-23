/*
 * A115813  Integers i such that 2*i XOR 5*i = 7*i.
 * https://oeis.org/A115813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers i such that (2*i) XOR (5*i) = 7*i */
#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache  = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;   /* next integer to test          */

    /* ensure we have at least n+1 terms stored */
    if ((size_t)n >= cache_len) {
        size_t target = (size_t)n + 1;

        /* grow cache if necessary */
        if (cache_cap < target) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < target) new_cap = target;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* search for new terms until we have enough */
        while (cache_len <= (size_t)n) {
            seq_t val = (seq_t)next_candidate;
            /* test the property: (2*val) XOR (5*val) == 7*val  
             * equivalent to (2*val) & (5*val) == 0          */
            if (((val << 1) & (val * (seq_t)5)) == 0)
                cache[cache_len++] = val;
            next_candidate++;
        }
    }

    return cache[(size_t)n];
}
