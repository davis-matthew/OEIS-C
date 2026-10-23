/*
 * A123013  a(n) = A122192(n)/6.
 * https://oeis.org/A123013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=0,a(2)=-253,a(3)=-1288,a(4)=191521,a(5)=1629320;
   for n>=6, a(n) = -(759*a(n-2)+2576*a(n-3)+759*a(n-4)+a(n-6)) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 5;          /* highest index computed so far */

    /* base values */
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)0,
        (seq_t)-253,
        (seq_t)-1288,
        (seq_t)191521,
        (seq_t)1629320
    };

    /* initialise or grow cache */
    if (!cache) {
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 6) init_cap = 6;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        memcpy(cache, base, 6 * sizeof(seq_t));   /* copy base values */
        cap = init_cap;
        last = 5;
    } else if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if ((idx_t)last < n) {
        for (idx_t i = (idx_t)last + 1; i <= n; ++i) {
            if (i < 6) {
                cache[i] = base[(size_t)i];
                continue;
            }
            cache[i] = - (cache[i-2] * (seq_t)759
                          + cache[i-3] * (seq_t)2576
                          + cache[i-4] * (seq_t)759
                          + cache[i-6]);
        }
        last = n;
    }

    return cache[n];
}
