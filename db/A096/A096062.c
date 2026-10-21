/*
 * A096062  a(1) = a(2) = 1; for n > 2, if a(n-2) + a(n-1) > n then a(n) =
 *          abs(a(n-2) - a(n-1)) else a(n) = a(n-2) + a(n-1).
 * https://oeis.org/A096062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096062: a(1) = a(2) = 1;
   for n>2, if a(n‑2)+a(n‑1) > n then a(n)=|a(n‑2)-a(n‑1)| else a(n)=a(n‑2)+a(n‑1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* largest n computed so far */

    /* handle trivial cases */
    if (n < 1) return 0;                 /* undefined, return 0 */
    if (n == 1 || n == 2) {
        if (max_n < 2) {                  /* ensure cache has indices 1,2 */
            idx_t newsize = 2;
            cache = (seq_t *)realloc(cache, (newsize + 1) * sizeof(seq_t));
            cache[1] = 1;
            cache[2] = 1;
            max_n = 2;
        }
        return cache[n];
    }

    /* extend cache if needed */
    if (n > max_n) {
        idx_t newsize = n;
        cache = (seq_t *)realloc(cache, (newsize + 1) * sizeof(seq_t));

        /* compute values from max_n + 1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];     /* a(n-1) */
            seq_t a2 = cache[i - 2];     /* a(n-2) */
            seq_t sum = a1 + a2;

            if (sum > (seq_t)i) {        /* use seq_t for comparison */
                /* abs difference */
                cache[i] = a1 > a2 ? a1 - a2 : a2 - a1;
            } else {
                cache[i] = sum;
            }
        }
        max_n = n;
    }

    return cache[n];
}
