/*
 * A069909  Numbers congruent to +-1, +-4, +-6, +-7 mod 16.
 * https://oeis.org/A069909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069909: numbers congruent to ±1, ±4, ±6, ±7 mod 16
   Recurrence: a(0)=1, a(1)=4, a(2)=6, a(3)=7, a(4)=9, a(5)=10,
                a(n)=2*a(n-1)-a(n-2)-a(n-4)+2*a(n-5)-a(n-6)   (n ≥ 6)
   The function memoises results so any order of calls works.
   It works for regular integer types (C/C++ builds) and for the
   arbitrary‑precision seq_t class used in the C++ build.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms: cache[0..cached-1] */
    static idx_t cached = 0;      /* number of cached entries */

    /* extend cache if needed */
    if (n >= cached) {
        idx_t new_size = n + 1;   /* we need indices 0 … n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;

        /* initialise new entries */
        for (idx_t i = cached; i < new_size; ++i) {
            if (i == 0)  cache[i] = (seq_t)1;
            else if (i == 1)  cache[i] = (seq_t)4;
            else if (i == 2)  cache[i] = (seq_t)6;
            else if (i == 3)  cache[i] = (seq_t)7;
            else if (i == 4)  cache[i] = (seq_t)9;
            else if (i == 5)  cache[i] = (seq_t)10;
            else {
                /* recurrence for n ≥ 6 */
                seq_t val = 2 * cache[i-1]
                          - cache[i-2]
                          - cache[i-4]
                          + 2 * cache[i-5]
                          - cache[i-6];
                cache[i] = val;
            }
        }
        cached = new_size;
    }
    return cache[n];
}
