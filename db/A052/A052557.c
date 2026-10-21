/*
 * A052557  Expansion of e.g.f. (1-x)/(1-x-x^3).
 * https://oeis.org/A052557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)*(n-2)*a(n-3) Initial terms: a(0)=1,
 *            a(1)=0, a(2)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Uses static cache; works even if a(n) is called out of order */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* pointer to cache array */
    static idx_t  cached = 0;       /* number of entries currently cached */

    /* Grow cache to include index n if necessary */
    if (cached <= n) {
        idx_t   new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)            /* allocation failure – return 0 */
            return (seq_t)0;

        /* Compute new entries */
        for (idx_t i = cached; i < new_len; ++i) {
            if (i == 0) {
                new_cache[i] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                new_cache[i] = (seq_t)0;
            } else {
                seq_t t1 = ((seq_t)i) * new_cache[i-1];
                seq_t t2 = ((seq_t)i) *
                           ((seq_t)(i-1)) *
                           ((seq_t)(i-2)) *
                           new_cache[i-3];
                new_cache[i] = t1 + t2;
            }
        }

        cache   = new_cache;
        cached  = new_len;
    }
    return cache[n];
}
