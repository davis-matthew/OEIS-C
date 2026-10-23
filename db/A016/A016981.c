/*
 * A016981  Expansion of 1/((1-3x)(1-4x)(1-10x)).
 * https://oeis.org/A016981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016981: Expansion of 1/((1-3x)(1-4x)(1-10x))
   Recurrence: a(0)=1, a(1)=17, a(2)=207, a(n)=17*a(n-1)-82*a(n-2)+120*a(n-3) (n≥3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation cache */
    static size_t cap   = 0;      /* allocated size of cache       */
    static idx_t  last  = (idx_t)-1;  /* largest index that has a value */

    /* Allocate or grow the cache to hold index n */
    if (cap == 0 || n >= cap)
    {
        size_t newcap = (size_t)n + 1;          /* at least n+1 elements */
        if (newcap < 3) newcap = 3;             /* need space for the three initial terms */

        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);                      /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first three terms if they are not already set */
    if (last == (idx_t)-1)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)207;
        last = 2;
    }

    /* Return the value if it has already been computed */
    if (n <= last)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        /* 17 * a1  */
        seq_t term = (a1 << 4) + a1;

        /* + 120 * a3  (a3*64 + a3*32 + a3*16 + a3*8) */
        term += (a3 << 6) + (a3 << 5) + (a3 << 4) + (a3 << 3);

        /* - 82 * a2  (a2*64 + a2*16 + a2*2) */
        term -= (a2 << 6) + (a2 << 4) + (a2 << 1);

        cache[i] = term;
    }

    last = n;
    return cache[n];
}
