/*
 * A037103  Trajectory of 3 under map n->11n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; for n>0, a(n)= (a(n-1)%2==0) ? a(n-1)/2 : 11*a(n-1)+1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       `used` is the number of stored terms (valid indices are 0..used-1),
       `cap` is the allocation size. */
    static seq_t *cache = NULL;
    static idx_t used = 0, cap = 0;

    /* Grow cache to at least n+1 elements. */
    if (used <= n)
    {
        /* Ensure capacity. */
        if (cap <= n)
        {
            idx_t new_cap = cap ? cap * 2 : 2;
            if (new_cap <= n) new_cap = n + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
            cap   = new_cap;
        }

        /* Initialize first term if the cache was empty. */
        if (used == 0)
        {
            cache[0] = (seq_t)3;
            used = 1;
        }

        /* Compute missing terms up to index n. */
        for (idx_t i = used; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t next;
            if ((prev & ((seq_t)1)) == ((seq_t)0))
                next = prev / ((seq_t)2);
            else
                next = prev * ((seq_t)11) + ((seq_t)1);
            cache[i] = next;
        }
        used = n + 1;
    }

    return cache[n];
}
