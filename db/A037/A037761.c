/*
 * A037761  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037761: Base‑7 digits are the first n terms of the periodic
   sequence with initial period 3,0,1,2.  The recurrence relation is
   a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5) with a(1)=3, a(2)=21, a(3)=148,
   a(4)=1038, a(5)=7269. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1 – values for n < 1 are not required */
    if (n < 1) return (seq_t)0;

    /* static cache for dynamic programming */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;      /* allocated number of slots */
    static size_t cache_max = 0;      /* highest index computed so far */

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (cache_max < (size_t)n) {
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
            if (i == 1)          cache[i] = (seq_t)3;
            else if (i == 2)     cache[i] = (seq_t)21;
            else if (i == 3)     cache[i] = (seq_t)148;
            else if (i == 4)     cache[i] = (seq_t)1038;
            else if (i == 5)     cache[i] = (seq_t)7269;
            else {
                seq_t t1 = cache[i-1];
                t1 *= (seq_t)7;
                seq_t t2 = cache[i-4];
                seq_t t3 = cache[i-5];
                t3 *= (seq_t)7;
                cache[i] = t1 + t2 - t3;
            }
        }
        cache_max = (size_t)n;
    }

    return cache[(size_t)n];
}
