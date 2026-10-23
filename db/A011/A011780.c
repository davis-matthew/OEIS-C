/*
 * A011780  Expansion of 1/((1-2*x)^3*(1-x^2)^2).
 * https://oeis.org/A011780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) -10*a(n-2) -4*a(n-3) +23*a(n-4)
                     -10*a(n-5) -12*a(n-6) +8*a(n-7);
   initial values: a(0)=1, a(1)=6, a(2)=26, a(3)=92, a(4)=291, a(5)=850, a(6)=2348
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize results */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Initialise base values on first call */
    if (!cache)
    {
        cap = 8;   /* Minimum capacity to hold indices up to 7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 6;   /* a(1) */
        cache[2] = 26;  /* a(2) */
        cache[3] = 92;  /* a(3) */
        cache[4] = 291; /* a(4) */
        cache[5] = 850; /* a(5) */
        cache[6] = 2348;/* a(6) */
        len = 7;       /* number of terms computed so far */
    }

    /* Return cached value if already computed */
    if (n < len)
        return cache[n];

    /* Grow cache if needed */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms using recurrence */
    for (idx_t i = len; i <= n; ++i)
    {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];
        seq_t t7 = cache[i - 7];

        seq_t val = 0;
        val += 6 * t1;
        val -= 10 * t2;
        val -= 4 * t3;
        val += 23 * t4;
        val -= 10 * t5;
        val -= 12 * t6;
        val += 8 * t7;

        cache[i] = val;
    }

    len = n + 1;
    return cache[n];
}
