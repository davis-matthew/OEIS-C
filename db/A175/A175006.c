/*
 * A175006  Row sums of triangle A175009.
 * https://oeis.org/A175006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 607, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5)
   - 3*a(n-6) + a(n-7)   for n > 7
   Initial values: a(1)=1, a(2)=3, a(3)=9, a(4)=21, a(5)=44, a(6)=81, a(7)=139
   Offset: 1   (a(1) is the first term). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0;   /* highest n computed so far */
    static idx_t  capacity = 0;  /* allocated size (size-1 is max index) */

    if (n == 0) return 0; /* undefined, but safe */

    /* initialise cache if needed */
    if (!cache) {
        capacity = (n > 7) ? n : 8;
        cache = (seq_t *)calloc((size_t)(capacity + 1), sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)44;
        cache[6] = (seq_t)81;
        cache[7] = (seq_t)139;
        cached = 7;
    }

    /* grow allocation if required */
    if (n > capacity) {
        capacity = n;
        cache = (seq_t *)realloc(cache, (size_t)(capacity + 1) * sizeof(seq_t));
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i <= 7) {
            /* values already initialised */
            continue;
        }
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];
        seq_t a7 = cache[i-7];

        /* compute val = 3*a1 - a2 - 5*a3 + 5*a4 + a5 - 3*a6 + a7
           avoid underflow by ordering additions/subtractions */
        seq_t val = 3*a1;
        val += 5*a4;
        val += a5;
        val -= a2;
        val -= 5*a3;
        val -= 3*a6;
        val += a7;

        cache[i] = val;
    }
    cached = n;
    return cache[n];
}
