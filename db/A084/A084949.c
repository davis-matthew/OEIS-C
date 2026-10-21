/*
 * A084949  a(n) = Product_{i=0..n-1} (9*i+2).
 * https://oeis.org/A084949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=0}^{n‑1}(9*i+2) Recurrence: a(0)=1;
 *            a(n)=a(n‑1)*(9*(n-1)+2) for n>0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms */
    static idx_t  cap   = 0;      /* number of cached entries */

    /* required size of cache (indices 0 … n) */
    size_t needed = (size_t)n + 1;

    if (cap < needed) {
        /* enlarge cache */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */

        cache = tmp;

        /* compute new terms */
        for (idx_t i = cap; i < (idx_t)needed; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;          /* a(0)=1 */
            } else {
                seq_t prev   = cache[i-1];
                seq_t factor = (seq_t)(9 * (i-1) + 2);  /* term multiplying a(i-1) */
                cache[i] = prev * factor;
            }
        }
        cap = (idx_t)needed;
    }

    return cache[n];
}
