/*
 * A045406  A diagonal of A008296.
 * https://oeis.org/A045406
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 22, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045406 – diagonal of A008296
   Recurrence (valid for n >= 6):
     a(n) + (2*n - 7)*a(n-1) + (n-4)^2 * a(n-2) = 0
   Base values (OEIS offset 2):
     a(2) = 1,  a(3) = 3,  a(4) = -1,  a(5) = 0   (a(0)=a(1)=0 unofficial)
*/
#include <stdlib.h>
#include <string.h>   /* for memcpy */

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_up_to = 0;   /* highest computed index */

/* ensure the cache can hold indices up to n */
static void
ensure_cache (idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = (size_t)(n + 1) * 2;   /* grow linearly */
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return;                     /* out‑of‑memory: caller will fail */

    if (cache)
    {
        memcpy(new_arr, cache, (cached_up_to + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* public function required by the framework */
seq_t
a (idx_t n)
{
    if (n < 2)
        return 0;                   /* undefined for n < 2, return 0 */

    ensure_cache(n);

    /* initialise base values once */
    if (cached_up_to < 5)
    {
        /* zero lower indices for safety */
        for (idx_t i = 0; i <= 5; ++i)
            cache[i] = 0;

        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)-1;
        cache[5] = (seq_t)0;

        cached_up_to = 5;
    }

    /* compute values up to n using the recurrence */
    for (idx_t i = cached_up_to + 1; i <= n; ++i)
    {
        /* recurrence holds for i >= 6 */
        seq_t coeff1 = (seq_t)(2 * i - 7);   /* (2*i - 7) */
        seq_t coeff2 = (seq_t)(i - 4);       /* (i - 4)   */
        seq_t term = - ( coeff1 * cache[i - 1]
                       + coeff2 * coeff2 * cache[i - 2] );
        cache[i] = term;
    }

    cached_up_to = n;
    return cache[n];
}
