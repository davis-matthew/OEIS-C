/*
 * A233253  Number of n X 5 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 512*a(n-1) - 33792*a(n-2) + 589824*a(n-3) with a(1) = 36,
 *            a(2) = 13952, a(3) = 6049792
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* off‑offset behaviour */

    static seq_t *cache = NULL;    /* cache[0] unused, cache[1]..cache[n] valid */
    static size_t allocated = 0;   /* number of allocated slots (>=1) */

    /* enlarge cache if necessary */
    if ((size_t)n >= allocated)
    {
        size_t new_alloc = (size_t)n + 1;        /* slots 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) return 0;                    /* allocation failure → 0 */
        cache = tmp;

        /* compute new entries */
        for (size_t i = allocated; i < new_alloc; ++i)
        {
            if (i == 0) continue;             /* skip unused 0 slot */

            if (i == 1)
                cache[i] = (seq_t)36;
            else if (i == 2)
                cache[i] = (seq_t)13952;
            else if (i == 3)
                cache[i] = (seq_t)6049792;
            else
                cache[i] = (seq_t)512 * cache[i-1]
                         - (seq_t)33792 * cache[i-2]
                         + (seq_t)589824 * cache[i-3];
        }
        allocated = new_alloc;
    }

    return cache[(size_t)n];
}
