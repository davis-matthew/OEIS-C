/*
 * A030036  a(n+1) = Sum_{k=0..floor(n/5)} a(k) * a(n-k).
 * https://oeis.org/A030036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 39, u64 n <= 70, u128 n <= 131,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 976 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   a(n+1) = Σ_{k=0..⌊n/5⌋} a(k) * a(n-k)   (OEIS A030036) */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t upper = 0;   /* highest index computed so far */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache[0] = (seq_t)1;
        upper = 0;
    }

    /* compute up to n if needed */
    if (n > upper) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        cache = tmp;

        for (idx_t m = upper + 1; m <= n; ++m) {
            if (m == 0) {                       /* safety for m==0 */
                cache[0] = (seq_t)1;
                continue;
            }
            idx_t limit = (m - 1) / 5;           /* floor((m-1)/5) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= limit; ++k) {
                sum += cache[k] * cache[m - 1 - k];
            }
            cache[m] = sum;
        }
        upper = n;
    }

    return cache[n];
}
