/*
 * A115661  Numbers k which have the same number of distinct prime divisors as
 *          the reverse of k.
 * https://oeis.org/A115661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that ω(k) = ω(reverse(k)) (OEIS A115661). */
/* The implementation searches forwards, storing results
   in a static cache to handle arbitrary calls in any order. */

#include <stdlib.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* static cache for found terms */
    static seq_t *cache   = NULL;   /* array of seq_t values */
    static size_t cache_len = 0;    /* number of stored terms */
    static size_t cache_cap = 0;    /* capacity of the array */
    static idx_t  last_checked = 0; /* last integer examined */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        ++last_checked;                       /* candidate k */
        int omega_k   = count_distinct_primes(last_checked);
        int omega_rev = count_distinct_primes(rev(last_checked));
        if (omega_k == omega_rev)
        {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)last_checked;  /* store term */
            ++cache_len;
        }
    }
    return cache[n - 1];
}
