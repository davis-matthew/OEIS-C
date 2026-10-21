/*
 * A222341  Number of (n+4) X 1 arrays of occupancy after each element moves
 *          up to +-4 places including 0.
 * https://oeis.org/A222341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
   Initial values: a(1)=126, a(2)=460, a(3)=1690, a(4)=6225, a(5)=22950
   Offset: 1 (a(1) is the first term) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined for n >= 1 only */

    static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1) ... */
    static size_t cap   = 0;      /* allocated capacity (number of slots) */
    static size_t max   = 0;      /* highest index that has been computed */

    /* ensure we have space for indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (needed < 6) needed = 6;   /* must hold at least the first 5 values */

    if (!cache) {                 /* first call: allocate and initialize */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        cap = needed;
        cache[0] = 0;                  /* unused slot */
        cache[1] = (seq_t)126;
        cache[2] = (seq_t)460;
        cache[3] = (seq_t)1690;
        cache[4] = (seq_t)6225;
        cache[5] = (seq_t)22950;
        max = 5;
    } else if (needed > cap) {    /* grow cache if necessary */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* reallocation failed */
        cache = tmp;
        cap = needed;
    }

    /* compute new values if requested n is beyond what we have */
    if ((size_t)n > max) {
        size_t i;
        for (i = max + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] * (seq_t)9
                     - cache[i-2] * (seq_t)28
                     + cache[i-3] * (seq_t)35
                     - cache[i-4] * (seq_t)15
                     + cache[i-5];
        }
        max = (size_t)n;
    }

    return cache[n];
}
