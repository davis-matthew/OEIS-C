/*
 * A077931  Expansion of 1/((1-x)*(1+2*x+x^2+2*x^3)).
 * https://oeis.org/A077931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-2) - a(n-3) + 2*a(n-4) for n ≥ 4 Initial
 *            values: a(0) = 1, a(1) = -1, a(2) = 2, a(3) = -4
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;  /* cache of computed terms          */
    static idx_t  cap    = 0;      /* allocated array size             */
    static idx_t  last   = (idx_t)-1;   /* last index that has been computed */

    /* Ensure cache large enough for index n */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure → undefined on this build */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first few terms if nothing is cached yet */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t) 1;
        if (n >= 1) cache[1] = (seq_t)-1;
        if (n >= 2) cache[2] = (seq_t) 2;
        if (n >= 3) cache[3] = (seq_t)-4;
        last = n < 3 ? n : 3;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i < 4) {          /* should not be needed after init, but be safe */
            switch (i) {
                case 0: cache[0] = (seq_t) 1; break;
                case 1: cache[1] = (seq_t)-1; break;
                case 2: cache[2] = (seq_t) 2; break;
                case 3: cache[3] = (seq_t)-4; break;
            }
        } else {
            cache[i] = -cache[i-1] + cache[i-2] - cache[i-3] + (seq_t)2 * cache[i-4];
        }
    }
    last = n;
    return cache[n];
}
