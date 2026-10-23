/*
 * A011662  A binary m-sequence: expansion of reciprocal of x^5 + x^2 + 1.
 * https://oeis.org/A011662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) XOR a(n-5) for n >= 5, with a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for the recurrence */
    static const seq_t init[5] = {0, 0, 0, 0, 1};

    /* Dynamic cache for previously computed terms */
    static seq_t *cache    = NULL;
    static size_t cap      = 0;   /* allocated capacity of cache   */
    static size_t len      = 0;   /* number of terms that are currently stored (cache[0..len-1]) */

    /* --- initial allocation and seeding of cache with the first 5 terms --- */
    if (cache == NULL)
    {
        /* start with a conservative capacity (at least 5) */
        cap = 64;
        while (cap < 5) cap <<= 1;

        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);          /* allocation failed */

        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];

        len = 5;                         /* we have stored the first 5 terms */
    }

    /* --- ensure we have enough space for the requested index --- */
    if (n + 1 > cap)                       /* (+1 because indices are 0‑based) */
    {
        size_t newCap = cap;
        while (newCap <= n)
            newCap <<= 1;                  /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);          /* allocation failed */

        cache = tmp;
        cap   = newCap;
    }

    /* --- compute missing terms (recurrence a(n) = a(n-2) ^ a(n-5)) --- */
    if (n + 1 > len)                       /* need to extend cache up to n */
    {
        for (idx_t i = len; i <= n; ++i)
            cache[i] = cache[i - 2] ^ cache[i - 5];

        len = n + 1;
    }

    /* --- return the requested term --- */
    return cache[n];
}
