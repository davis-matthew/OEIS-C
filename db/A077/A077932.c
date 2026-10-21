/*
 * A077932  Expansion of (1-x)^(-1)/(1+2*x+2*x^2-2*x^3).
 * https://oeis.org/A077932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=-1, a(2)=1, a(3)=3,
 * recurrence: a(n) = -a(n-1) + 4*a(n-3) - 2*a(n-4)   (n >= 4)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache, grows when needed */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated array size   */
    static idx_t   sz    = 0;   /* number of terms stored */

    /* first call: allocate and initialise first terms */
    if (cache == NULL) {
        cap = 16;                      /* start with some room */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)-1;          /* a(1) */
        cache[2] = (seq_t)1;           /* a(2) */
        cache[3] = (seq_t)3;           /* a(3) */
        sz = 4;                        /* 4 terms stored */
    }

    /* grow array if we need to compute more terms */
    while (sz <= n) {
        if (sz == cap) {               /* double capacity */
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cap = newcap;
        }

        /* compute term cache[sz] using recurrence */
        seq_t term = -(cache[sz - 1])               // -a(n-1)
                   + (seq_t)4 * cache[sz - 3]       // +4*a(n-3)
                   - (seq_t)2 * cache[sz - 4];      // -2*a(n-4)
        cache[sz] = term;
        ++sz;
    }

    return cache[(size_t)n];
}
