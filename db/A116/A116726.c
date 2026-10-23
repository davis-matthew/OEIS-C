/*
 * A116726  Number of permutations of length n which avoid the patterns 213,
 *          1234, 2431.
 * https://oeis.org/A116726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116726: Number of permutations of length n avoiding patterns
   213, 1234, 2431.
   Recurrence (for n ≥ 6):
     a(n) = 3*a(n-1) – a(n-2) – 2*a(n-3) – a(n-4) + a(n-5) + a(n-6)
   Initial values (with a(0) = 0 for convenience):
     a(1)=1, a(2)=2, a(3)=5, a(4)=12, a(5)=26. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* initialise cache on first call */
    if (!cache) {
        cache_sz = (n > 5 ? n + 1 : 6);              /* at least indices 0..5 */
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */
        cache[0] = 0;                               /* a(0) unused but set to 0 */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 5;
        cache[4] = 12;
        cache[5] = 26;
    }

    /* expand cache if necessary */
    if (n >= cache_sz) {
        idx_t old_sz = cache_sz;
        cache_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_sz * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failure */
        cache = tmp;

        for (idx_t i = old_sz; i <= n; ++i) {
            if (i <= 5) continue;                  /* base values already set */

            /* Compute using the recurrence:
               sum   = 3*a(i-1) + a(i-5) + a(i-6)
               sub   = a(i-2) + 2*a(i-3) + a(i-4)
               a(i)  = sum - sub
            */
            seq_t sum = cache[i-1];
            sum = sum + sum + sum;                 /* 3*a(i-1) */
            sum = sum + cache[i-5];
            sum = sum + cache[i-6];

            seq_t sub = cache[i-2];
            sub = sub + cache[i-3] + cache[i-3];   /* 2*a(i-3) */
            sub = sub + cache[i-4];

            cache[i] = sum - sub;
        }
    }

    /* a(0) is outside the required offset */
    if (n < 1) return 0;
    return cache[n];
}
