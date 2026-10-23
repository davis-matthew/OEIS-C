/*
 * A190569  Number of n-step one-sided prudent walks, avoiding single west
 *          steps and single east steps.
 * https://oeis.org/A190569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   A190569(a(n)) = A007909(n+2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3; for n>=3: a(n)=2*a(n-1)-a(n-2)+2*a(n-3). */

#include <stdlib.h>

/* cache for memoised values */
static seq_t *seq_cache = NULL;   /* dynamic array of computed terms */
static idx_t  cache_cap = 0;      /* number of terms currently stored (<= cache_cap-1) */

/* return the nth term of A190569 */
seq_t a(idx_t n)
{
    /* if already computed, return it */
    if (n < cache_cap)
        return seq_cache[n];

    /* need to extend the cache up to index n */
    idx_t new_cap = n + 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache)
        return 0;  /* allocation failure -- caller must not rely on this value */

    /* compute all terms from cache_cap up to n */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0)
            seq_cache[i] = (seq_t)1;
        else if (i == 1)
            seq_cache[i] = (seq_t)1;
        else if (i == 2)
            seq_cache[i] = (seq_t)3;
        else {
            seq_t t = seq_cache[i-1] << 1;     /* 2 * a(i-1) */
            t -= seq_cache[i-2];              /*   - a(i-2) */
            t += (seq_cache[i-3] << 1);       /*   + 2 * a(i-3) */
            seq_cache[i] = t;
        }
    }

    cache_cap = new_cap;
    return seq_cache[n];
}
