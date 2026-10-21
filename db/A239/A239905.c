/*
 * A239905  If n <= 5 then a(n) = 1, if 6 <= n <= 8 then 2, if n = 9 or 10
 *          then 3, if n = 11, 12 or 13 then n-7; otherwise a(n) = 2*a(n - 4)
 *          + a(n - 12).
 * https://oeis.org/A239905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1..5) = 1 a(6..8) = 2 a(9..10) = 3 a(11..13) = n - 7 for n >=
 *            14 : a(n) = 2*a(n-4) + a(n-12)
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 116, u64 n <= 228,
 *            u128 n <= 452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety guard */

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t  cap        = 0;             /* allocated size (elements) */
    static idx_t  last_filled = 0;            /* highest index computed (0‑based) */

    /* grow cache if necessary */
    if (cap < n) {
        seq_t *newp = (seq_t *)malloc(n * sizeof(seq_t));
        if (!newp) { /* allocation failure */ return 0; }
        if (cache) {
            /* copy previous data */
            memcpy(newp, cache, last_filled * sizeof(seq_t));
            free(cache);
        }
        cache = newp;
        cap   = n;
    }

    /* compute new values up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 5) {
            val = 1;
        } else if (i <= 8) {
            val = 2;
        } else if (i <= 10) {
            val = 3;
        } else if (i <= 13) {
            val = (seq_t)(i - 7);
        } else {
            /* a(i) = 2*a(i-4) + a(i-12) */
            seq_t prev4 = cache[i - 5];   /* a(i-4) stored at index i-5 */
            seq_t prev12 = cache[i - 13]; /* a(i-12) stored at index i-13 */
            val = prev4 * 2;
            val += prev12;
        }
        cache[i - 1] = val;
    }

    last_filled = n;
    return cache[n - 1];
}
