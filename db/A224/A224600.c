/*
 * A224600  Number of (n+2)X4 0..2 matrices with each 3X3 subblock idempotent.
 * https://oeis.org/A224600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 46, u64 n <= 104, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224600
   Recurrence:  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) - a(n-5) + 2*a(n-6) - a(n-7)
   Initial values:
        a(1)=178, a(2)=272, a(3)=371, a(4)=449, a(5)=630,
        a(6)=894, a(7)=1221, a(8)=1707, a(9)=2440
 */
#include <stdlib.h>   /* malloc, realloc, abort */

extern seq_t a(idx_t n)
{
    /* 1‑indexed cache of already‑computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* number of allocated slots (>= n+1) */
    static idx_t max_computed = 0;     /* highest index with a value set */

    if (n < 1)            /* behaviour below offset is irrelevant */
        return 0;

    /* ensure the cache can hold index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failure – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first nine values once */
    if (max_computed < 9) {
        cache[1] = (seq_t)178;
        cache[2] = (seq_t)272;
        cache[3] = (seq_t)371;
        cache[4] = (seq_t)449;
        cache[5] = (seq_t)630;
        cache[6] = (seq_t)894;
        cache[7] = (seq_t)1221;
        cache[8] = (seq_t)1707;
        cache[9] = (seq_t)2440;
        max_computed = 9;
    }

    if (n <= max_computed)        /* already known */
        return cache[n];

    /* compute subsequent values using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = (seq_t)3 * cache[i-1]       /* 3*a(i-1)   */
                  + cache[i-4]                  /* +a(i-4)    */
                  + (seq_t)2 * cache[i-6];      /* +2*a(i-6)  */

        seq_t neg = (seq_t)2 * cache[i-2]       /* 2*a(i-2)   */
                  + cache[i-3]                  /* +a(i-3)    */
                  + cache[i-5]                  /* +a(i-5)    */
                  + cache[i-7];                 /* +a(i-7)    */

        cache[i] = pos - neg;                  /* pos >= neg */
    }
    max_computed = n;
    return cache[n];
}
