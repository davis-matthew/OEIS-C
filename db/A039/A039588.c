/*
 * A039588  Numbers whose base-6 representation has the same number of 1's,
 *          4's and 5's.
 * https://oeis.org/A039588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039588: numbers whose base‑6 representation has the same
 * number of 1's, 4's and 5's.  This implementation searches successive
 * integers and keeps the ones that satisfy the property. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;     /* array of found terms */
    static idx_t  cache_cap = 0;    /* capacity of cache */
    static idx_t  computed  = 0;    /* number of terms already computed */
    static seq_t  next_candidate = 0; /* next integer to test */

    /* sanity: return 0 for n==0 (not used, offset starts at 1) */
    if (n == 0) return (seq_t)0;

    /* enlarge cache if necessary */
    if (cache_cap < n)
    {
        cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        cache_cap = n;
    }

    /* generate terms until we have enough */
    while (computed < n)
    {
        seq_t val = next_candidate;
        seq_t tmp = val;
        int c1 = 0, c4 = 0, c5 = 0;

        /* Count digits 1,4,5 in base‑6 */
        while (tmp != (seq_t)0)
        {
            int d = (int)(tmp % (seq_t)6);
            if (d == 1) ++c1;
            else if (d == 4) ++c4;
            else if (d == 5) ++c5;
            tmp /= (seq_t)6;
        }

        /* term qualifies if the three counts are equal */
        if (c1 == c4 && c4 == c5)
        {
            cache[computed++] = val;
        }

        /* advance to next integer */
        ++next_candidate;
    }

    return cache[n - 1];
}
