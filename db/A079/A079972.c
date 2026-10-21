/*
 * A079972  Number of permutations satisfying -k <= p(i)-i <= r and p(i)-i not
 *          in I, i=1..n, with k=1, r=4, I={1,2}.
 * https://oeis.org/A079972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) + a(n-5) (with a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, and a(k)=0 for k<0)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 56, u64 n <= 111,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* static cache of computed values */
static idx_t   a_cache_len = 0; /* number of values currently stored (size of a_cache) */

seq_t a(idx_t n)
{
    /* Ensure cache holds values up to n */
    if (n >= a_cache_len)
    {
        idx_t new_len = n + 1;                 /* need space for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp)                               /* allocation failure -> abort */
            abort();

        /* Initialise new entries using recurrence */
        if (a_cache_len == 0)                   /* first call – set base values */
        {
            tmp[0] = (seq_t)1;
            /* for i=1..3 only a(i-1) term applies */
            for (idx_t i = 1; i <= 3 && i <= n; ++i)
                tmp[i] = tmp[i - 1];
            a_cache_len = 4;                    /* we have filled indices 0..3 */
        }

        /* Fill remaining indices */
        for (idx_t i = a_cache_len; i < new_len; ++i)
        {
            seq_t val = tmp[i - 1];             /* a(i-1) term (i>=1) */
            if (i >= 4) val += tmp[i - 4];
            if (i >= 5) val += tmp[i - 5];
            tmp[i] = val;
        }
        a_cache = tmp;
        a_cache_len = new_len;
    }

    return a_cache[n];
}
