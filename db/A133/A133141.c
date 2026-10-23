/*
 * A133141  Numbers which are both centered pentagonal (A005891) and centered
 *          hexagonal numbers (A003215).
 * https://oeis.org/A133141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=331, a(n)=482*a(n-1)-a(n-2)-150 for n>=3 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; for safety return 0 */
        return (seq_t)0;

    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise base terms once */
    if (cache_len == 0) {
        cache_len = 2;                     /* a(1) and a(2) stored */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;               /* a(1) */
        cache[1] = (seq_t)331;             /* a(2) */
    }

    /* if requested value already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache to needed size */
    cache = (seq_t*)realloc(cache, n * sizeof(seq_t));

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prevprev = cache[i - 2];     /* a(i-1)  */
        seq_t prev     = cache[i - 1];     /* a(i)    */
        cache[i] = (seq_t)482 * prev - prevprev - (seq_t)150;
    }
    cache_len = n;
    return cache[n - 1];
}
