/*
 * A037597  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊4^(n+1)/9⌋, a(1)=1, a(2)=7, a(3)=28, a(4)=113 Offsets:
 *            a(1) is the first term. Recurrence: a(n) =
 *            4*a(n-1)+a(n-3)-4*a(n-4) for n≥5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       cache[1] … cache[max_n] are defined. cache[0] is unused.   */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;     /* allocated size (≥ max_n+1)          */
    static idx_t max_n = 0;     /* largest n for which cache[n] is valid */

    if (n == 0)   /* offset starts at 1 – caller should not request n=0 */
        return 0;

    /* Grow cache if necessary.  We allocate at least one extra slot for
       index n itself, and double the capacity when many new values are
       required to keep amortised O(1) per call.   */
    if (n > max_n) {
        idx_t need  = n + 1;                     /* include index 0 */
        idx_t newcap = (cap < need) ? need : cap * 2 + 4;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)  /* allocation failure – return 0 (undefined sequence) */
            return 0;
        cache = tmp;
        cap   = newcap;

        /* Compute missing values up to n.  Base cases are hard‑coded.  */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1)          cache[i] = 1;
            else if (i == 2)     cache[i] = 7;
            else if (i == 3)     cache[i] = 28;
            else if (i == 4)     cache[i] = 113;
            else {
                seq_t a1 = cache[i - 1];
                seq_t a3 = cache[i - 3];
                seq_t a4 = cache[i - 4];
                cache[i] = 4 * a1 + a3 - 4 * a4;
            }
        }
        max_n = n;
    }

    return cache[n];
}
