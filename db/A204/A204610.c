/*
 * A204610  Number of (n+1) X 3 0..1 arrays with the determinants of 2 X 2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A204610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 5*a(n-2) - 12*a(n-3) + 12*a(n-4)  (n > 7)
   Initial values: a(1)=47, a(2)=181, a(3)=749, a(4)=3254,
   a(5)=14199, a(6)=62031, a(7)=271125 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; behaviour below offset is irrelevant */
        return 0;

    static seq_t *cache      = NULL;   /* 1‑based array of computed values              */
    static idx_t  cap        = 0;      /* current capacity of cache                    */
    static idx_t  last       = 0;      /* highest n that has been computed             */

    /* Ensure cache large enough for index n                           */
    if (cap < n + 1) {
        idx_t newcap = (cap ? cap * 2 : 8);
        while (newcap <= n) newcap *= 2;

        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – return 0 as sentinel    */

        if (cache) {                     /* copy old values to new cache               */
            for (idx_t i = 0; i <= last; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;                  /* capacity updated                           */
    }

    /* Initialise base cases if not yet done                           */
    if (last < 7) {
        cache[1] = 47;
        cache[2] = 181;
        cache[3] = 749;
        cache[4] = 3254;
        cache[5] = 14199;
        cache[6] = 62031;
        cache[7] = 271125;
        last = 7;
    }

    /* Compute missing values up to n                                  */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v = cache[i - 1] * 6;
        v += cache[i - 4] * 12;
        v -= cache[i - 2] * 5;
        v -= cache[i - 3] * 12;
        cache[i] = v;
    }
    if (n > last) last = n;

    return cache[n];
}
