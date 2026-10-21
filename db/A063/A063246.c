/*
 * A063246  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          99 ).
 * https://oeis.org/A063246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n ≥ 7 with initial values
 *            a(1)=4, a(2)=12, a(3)=22, a(4)=28, a(5)=38, a(6)=46
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for n < 1 */

    /* static cache of already computed terms (1‑based indexing) */
    static seq_t *cache   = NULL;            /* array of seq_t values */
    static idx_t  cap     = 0;               /* allocated size (≥ n+1) */
    static idx_t  max_computed = 0;          /* highest index filled */

    /* enlarge cache if necessary */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        /* initialise new cells to 0 */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the known first six terms if they have not been set yet */
    static const seq_t init[7] = {
        0, 4, 12, 22, 28, 38, 46   /* init[0] unused */
    };
    for (idx_t i = 1; i <= 6 && i < cap; ++i)
        if (cache[i] == 0) cache[i] = init[i];

    /* compute any missing terms beyond what we have already */
    idx_t start = max_computed + 1;
    if (start < 7) start = 7;                /* recurrence defined from n = 7 */
    for (idx_t i = start; i <= n; ++i)
        cache[i] = cache[i-2] + cache[i-3] - cache[i-5];

    if (n > max_computed) max_computed = n;

    return cache[n];
}
