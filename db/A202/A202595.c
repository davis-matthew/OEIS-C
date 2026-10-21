/*
 * A202595  Number of (n+2) X 4 binary arrays avoiding patterns 001 and 000 in
 *          rows and columns.
 * https://oeis.org/A202595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 22*a(n-2) + 12*a(n-3) - 43*a(n-4)
                     - 8*a(n-5) + 18*a(n-6) - a(n-8)
   Initial values (for n = 1 … 8):
   a(1)=510,  a(2)=3150,  a(3)=18160,  a(4)=107510,
   a(5)=629790, a(6)=3704980, a(7)=21758550, a(8)=127872530
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;      /* allocated capacity (number of slots)   */
    static idx_t  computed = 0;     /* highest index that has been computed    */

    /* Ensure cache exists and contains the base values up to n=8 */
    if (cache == NULL) {
        /* Allocate at least enough space for indices up to n or 8, whichever is larger */
        size_t init_cap = n + 1 > 9 ? (size_t)(n + 1) : 9;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cap = init_cap;

        /* initialise indices 0..8 (index 0 is unused) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)510;
        cache[2] = (seq_t)3150;
        cache[3] = (seq_t)18160;
        cache[4] = (seq_t)107510;
        cache[5] = (seq_t)629790;
        cache[6] = (seq_t)3704980;
        cache[7] = (seq_t)21758550;
        cache[8] = (seq_t)127872530;

        computed = 8;                     /* highest index with a known value */
    }

    /* Resize cache if a larger index is requested */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    if (n <= 8)            /* base values are already stored */
        return cache[n];

    /* Compute missing values up to n, using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)2  * cache[i-1]
                  + (seq_t)22 * cache[i-2]
                  + (seq_t)12 * cache[i-3]
                  - (seq_t)43 * cache[i-4]
                  - (seq_t)8  * cache[i-5]
                  + (seq_t)18 * cache[i-6]
                  -            cache[i-8];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
