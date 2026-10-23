/*
 * A123127  Coefficient of X^3 in the characteristic polynomial of the n-th
 *          power of the matrix M = {{1,1,1,1,1}, {1,0,0,0,0}, {0,1,0,0,0},
 *          {0,0,1,0,0}, {0,0,0,1,0}}.
 * https://oeis.org/A123127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 39, i64 n <= 81, i128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (order 10):
   a(n) = -a(n-1) -2a(n-2)-3a(n-3)-3a(n-4)+6a(n-5)
          -a(n-6)+a(n-7)+a(n-9)-a(n-10)   for n>10
   Initial values:
   a(1)=‑1, a(2)=‑3, a(3)=‑4, a(4)=1, a(5)=49,
   a(6)=‑42, a(7)=‑57, a(8)=‑31, a(9)=140, a(10)=497 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1 … 10 */
    static const seq_t base[10] = {
        (seq_t)-1,  /* a(1) */
        (seq_t)-3,  /* a(2) */
        (seq_t)-4,  /* a(3) */
        (seq_t)1,   /* a(4) */
        (seq_t)49,  /* a(5) */
        (seq_t)-42, /* a(6) */
        (seq_t)-57, /* a(7) */
        (seq_t)-31, /* a(8) */
        (seq_t)140, /* a(9) */
        (seq_t)497  /* a(10) */
    };

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* number of terms stored (≥10 after init) */

    /* Initialise cache on first call */
    if (cached_n == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 10);
        if (!cache) abort();              /* allocation failure */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        cached_n = 10;
    }

    /* Return value if already computed */
    if (n <= cached_n)
        return cache[n - 1];

    /* Grow cache to required size n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) abort();              /* allocation failure */
    cache = new_cache;

    /* Compute terms from cached_n+1 up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* recurrence:
           a(i) = -a(i-1) -2a(i-2)-3a(i-3)-3a(i-4)+6a(i-5)
                  -a(i-6)+a(i-7)+a(i-9)-a(i-10) */
        seq_t next = -cache[i - 2]                      /* -a(i-1) */
                   - (seq_t)2 * cache[i - 3]           /* -2a(i-2) */
                   - (seq_t)3 * cache[i - 4]           /* -3a(i-3) */
                   - (seq_t)3 * cache[i - 5]           /* -3a(i-4) */
                   + (seq_t)6 * cache[i - 6]           /* +6a(i-5) */
                   - cache[i - 7]                      /* -a(i-6) */
                   + cache[i - 8]                      /* +a(i-7) */
                   + cache[i - 10]                     /* +a(i-9) */
                   - cache[i - 11];                    /* -a(i-10) */

        cache[i - 1] = next;
    }

    cached_n = n;
    return cache[n - 1];
}
