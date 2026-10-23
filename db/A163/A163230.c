/*
 * A163230  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1)+42*a(n-2)+42*a(n-3)-903*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=44, a(2)=1892, a(3)=81356, a(4)=3497362 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of already computed terms   */
    static idx_t  cache_size = 0;    /* capacity of the cache             */
    static idx_t  computed   = 0;    /* number of terms that have been    */
                                     /* computed and stored in cache      */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                  /* out‑of‑memory – unlikely here */
        cache = tmp;
        cache_size = new_size;
    }

    /* if we have already computed the requested term, return it */
    if (computed > n)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i)
    {
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)1;                    break;
            case 1: val = (seq_t)44;                   break;
            case 2: val = (seq_t)1892;                 break;
            case 3: val = (seq_t)81356;                break;
            case 4: val = (seq_t)3497362;              break;
            default:
                /* recurrence: a(i) = 42*a(i-1)+42*a(i-2)+42*a(i-3)-903*a(i-4) */
                val = (cache[i-1] * (seq_t)42)
                    + (cache[i-2] * (seq_t)42)
                    + (cache[i-3] * (seq_t)42)
                    - (cache[i-4] * (seq_t)903);
                break;
        }
        cache[i] = val;
    }

    computed = n + 1;          /* we now have terms 0..n stored */
    return cache[n];
}
