/*
 * A084214  Inverse binomial transform of a math magic problem.
 * https://oeis.org/A084214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A084214
   Formula: a(n) = a(n-1) + 2*a(n-2)   for n > 2
   Initial values: a(0) = 1, a(1) = 1, a(2) = 4
   This recurrence follows from a(n) = (5*2^n + 4*(-1)^n)/6 for n>0.  */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms                    */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t  */
    static idx_t  cap     = 0;      /* currently allocated size  */
    static idx_t  last    = 0;      /* highest index already computed */
    static bool   init    = false;  /* whether base cases are set */

    /* allocate/reserve capacity if needed */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 4 : cap * 2;
        if ((idx_t)new_cap <= n) new_cap = n + 1;

        seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* out of memory – unlikely in tests */

        if (cache) {
            memcpy(new_cache, cache, (size_t)cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialize base cases once */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        last     = 2;
        init     = true;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] * (seq_t)2;
        }
        last = n;
    }

    return cache[n];
}
