/*
 * A080904  a(1)=1; for n>1, a(n)=a(n-1)-4 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A080904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computes OEIS A080904:  
   a(1) = 1; for n > 1,  
   a(n) = a(n-1) - 4 if n already appears among {a(1), …, a(n-1)}  
   a(n) = a(n-1) + 5 otherwise.  */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms: */ 
    static idx_t cache_len = 0;          /* number of terms currently stored */
    static seq_t *cache = NULL;          /* array 1..cache_len */

    /* Grow the cache to contain term n if necessary */
    if (n > cache_len)
    {
        size_t new_size = (size_t)(n + 1);    /* index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure -- not expected */
        cache = tmp;

        /* Compute new terms */
        for (idx_t k = cache_len + 1; k <= n; ++k)
        {
            if (k == 1)
            {
                cache[1] = 1;
            }
            else
            {
                bool seen = false;
                seq_t target = (seq_t)k;
                for (idx_t j = 1; j < k; ++j)
                {
                    if (cache[j] == target)
                    {
                        seen = true;
                        break;
                    }
                }
                if (seen)
                    cache[k] = cache[k - 1] - 4;
                else
                    cache[k] = cache[k - 1] + 5;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
