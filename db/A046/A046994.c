/*
 * A046994  Number of Greek-key tours on a 3 X n board; i.e., self-avoiding
 *          walks on a 3 X n grid starting in the top left corner.
 * https://oeis.org/A046994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=8, a(4)=17, a(n)=2*a(n-1)+2*a(n-2)-4*a(n-3)
 *            for n>4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 2384 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 0;   /* last index that has been computed */

    if (n < 1) return 0; /* undefined for n<1 */

    /* initialise cache if first call */
    if (cache == NULL) {
        cap   = 5;                       /* indices 0..4 will be stored */
        cache = mem_new(seq_t, cap);
        if (cache == NULL) return 0;     /* allocation failure */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)17;
        last    = 4;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (new_cache == NULL) return 0;   /* allocation failure */
        mem_copy(new_cache, cache, cap * sizeof(seq_t));
        mem_free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)3;
            } else if (i == 3) {
                cache[i] = (seq_t)8;
            } else if (i == 4) {
                cache[i] = (seq_t)17;
            } else {
                /* recurrence: a(n)=2*a(n-1)+2*a(n-2)-4*a(n-3) */
                cache[i] = cache[i-1] * ((seq_t)2)
                         + cache[i-2] * ((seq_t)2)
                         - cache[i-3] * ((seq_t)4);
            }
        }
        last = n;
    }

    return cache[n];
}
