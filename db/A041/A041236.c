/*
 * A041236  Numerators of continued fraction convergents to sqrt(130).
 * https://oeis.org/A041236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041236 - Numerators of continued fraction convergents to sqrt(130)
   Recurrence:
     a(0) = 11
     a(1) = 23          // 2*11 + 1
     For n >= 2:
       let a_term = (n % 3 == 0) ? 22 : 2
       a(n) = a_term * a(n-1) + a(n-2)
   The continued fraction of sqrt(130) is [11; (2,2,22) repeating].
   This implementation memoizes results up to the maximum index requested
   in any call, so later calls with larger n are fast. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoized terms a(0) .. a(max) */
    static idx_t cache_size = 0;   /* number of initialized entries */

    idx_t target = n;
    if (cache_size <= target) { /* need to grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(target + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed: return 0 (unlikely in tests) */
        cache = tmp;
        /* compute missing entries, starting from current cache_size */
        for (idx_t i = cache_size; i <= target; ++i) {
            seq_t ai;
            if (i == 0) {
                cache[0] = 11;  /* a0 */
            } else if (i == 1) {
                ai = 2; /* a1 term in recurrence */
                cache[1] = ai * cache[0] + 1; /* a1 = 2*a0 + 1 */
            } else {
                ai = (i % 3 == 0) ? 22 : 2;
                cache[i] = ai * cache[i-1] + cache[i-2];
            }
        }
        cache_size = target + 1;
    }
    return cache[n];
}
