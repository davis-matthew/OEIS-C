/*
 * A086223  Every integer can be represented uniquely as m = k*2^(j+1)+2^j-1.
 *          Sequence gives values of k for m = repunit(n).
 * https://oeis.org/A086223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(4)=69, a(5)=694; for n>=6:
 *            a(n)=11*a(n-1)-10*a(n-2) (exact integer arithmetic)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] holds a(1) */
static idx_t cache_len = 0;    /* number of terms currently stored */

/* Return the nth term of A086223.  n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety: callers guarantee n>=1 */

    /* Grow the cache if needed */
    if (cache_len < n)
    {
        idx_t new_len = n;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t val;
            if (i == 1)      val = (seq_t)0;
            else if (i == 2) val = (seq_t)1;
            else if (i == 3) val = (seq_t)3;
            else if (i == 4) val = (seq_t)69;
            else if (i == 5) val = (seq_t)694;
            else             /* i >= 6 */
            {
                /* a(i) = 11*a(i-1) - 10*a(i-2)  */
                val = ( (seq_t)11 * cache[i-2] ) - ( (seq_t)10 * cache[i-3] );
            }
            cache[i-1] = val;
        }

        cache_len = new_len;
    }

    return cache[n-1];
}
