/*
 * A212254  Number of (w,x,y,z) with all terms in {1,...,n} and w=x+2y+3z-n.
 * https://oeis.org/A212254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)-a(n-3)-a(n-4)+2*a(n-6)-a(n-7)
   with initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=4, a(5)=11, a(6)=21 */

#include <stdlib.h>

static const seq_t base_vals[7] = {0, 0, 0, 1, 4, 11, 21};
static seq_t *cache = NULL;   /* memoization array */
static idx_t cache_len = 0;   /* number of entries currently in cache */

/* Return the n-th term of A212254 */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(*cache));
        cache = tmp;  /* realloc always succeeds here or program terminates */
        /* Compute missing values */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i < 7)
            {
                cache[i] = base_vals[i];
            }
            else
            {
                seq_t val = cache[i - 1];
                val = val + val;                /* 2*a(i-1) */
                val = val - cache[i - 3];       /* -a(i-3) */
                val = val - cache[i - 4];       /* -a(i-4) */
                seq_t tmp2 = cache[i - 6];
                tmp2 = tmp2 + tmp2;             /* 2*a(i-6) */
                val = val + tmp2;
                val = val - cache[i - 7];       /* -a(i-7) */
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
