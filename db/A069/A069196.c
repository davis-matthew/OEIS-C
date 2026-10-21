/*
 * A069196  a(1)=1, a(2)=3; for n>1, a(n+2)=(a(n+1)+a(n))/3 if (a(n+1)+a(n)==0
 *          (mod 3)); a(n+2)=(a(n+1)+a(n))/2 if (a(n+1)+a(n)==0 (mod 2)); and
 *          a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=3; for n>1 let s = a(n)+a(n+1). Then a(n+2) = s/3
 *            if s % 3 == 0 a(n+2) = s/2 if s % 3 != 0 and s % 2 == 0 a(n+2) =
 *            s otherwise
 * Range:     u8 n <= 43, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t used = 0;        /* number of valid entries in cache */

    /* initialise first two terms */
    if (cache == NULL)
    {
        used = 2;
        cache = (seq_t *)malloc(used * sizeof(seq_t));
        if (!cache) abort();          /* unlikely but safety */
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)3;          /* a(2) */
    }

    /* if the requested term is already computed, return it */
    if (n <= used)
        return cache[n - 1];

    /* enlarge the cache so that it can hold up to n */
    idx_t new_cap = used * 2;
    if (new_cap < n)
        new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* memory exhausted */
    cache = tmp;

    /* compute missing terms up to n */
    for (idx_t i = used; i < n; ++i)
    {
        seq_t sum = cache[i - 1] + cache[i - 2];
        if (sum % (seq_t)3 == (seq_t)0)
            cache[i] = sum / (seq_t)3;
        else if (sum % (seq_t)2 == (seq_t)0)
            cache[i] = sum / (seq_t)2;
        else
            cache[i] = sum;
    }

    used = n;          /* update count of computed values */
    return cache[n - 1];
}
