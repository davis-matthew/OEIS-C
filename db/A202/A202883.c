/*
 * A202883  Number of n X 2 0..2 arrays with every nonzero element less than
 *          or equal to some horizontal or vertical neighbor.
 * https://oeis.org/A202883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 2*a(n-2) + 46*a(n-3) + 25*a(n-4) + 36*a(n-5),
 *            a(1) = 3, a(2) = 35, a(3) = 265, a(4) = 1849, a(5) = 13345
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (max index + 1) */
    static idx_t  last  = 0;   /* highest index that has been computed */

    if (n == 0)
        return 0;  /* undefined for n<1, guard against 0 */

    /* Ensure the cache is large enough for index n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = ((size_t)n + 1) * 2;      /* overshoot for future */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        cache = tmp;
        /* If the cache was previously empty, initialise index 0
           to a dummy value so that we can safely use cache[0] inside
           the recurrence (it will never be read for n >= 1). */
        if (cap == 0)
            cache[0] = (seq_t)0;
        cap = newcap;
    }

    /* Set the five initial values if not already set. */
    if (last < 5) {
        if (cap > 1) cache[1] = (seq_t)3;
        if (cap > 2) cache[2] = (seq_t)35;
        if (cap > 3) cache[3] = (seq_t)265;
        if (cap > 4) cache[4] = (seq_t)1849;
        if (cap > 5) cache[5] = (seq_t)13345;
        if (last < 5) last = 5;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = ((seq_t)6 * a1) + ((seq_t)2 * a2) +
                   ((seq_t)46 * a3) + ((seq_t)25 * a4) +
                   ((seq_t)36 * a5);
    }
    last = n;
    return cache[n];
}
