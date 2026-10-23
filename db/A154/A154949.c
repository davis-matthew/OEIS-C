/*
 * A154949  Diagonal sums of Riordan array A154948.
 * https://oeis.org/A154949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154949: Diagonal sums of Riordan array A154948
   Recurrence (derived from the generating function 
        G.f. = 1/((1-x^2)(1 - x - x^2 - x^3)) ):

   a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 5, a(4) = 10
   a(n) = a(n-1) + 2*a(n-2) - a(n-4) - a(n-5)   for n >= 5
*/

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values */
static idx_t    cache_len = 0;     /* number of allocated entries */

/* Initialise the cache with the first five terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache = (seq_t *)malloc(5 * sizeof(seq_t));
    if (cache == NULL)
        return;                    /* allocation failure – caller will see 0 */
    cache[0] = 1;
    cache[1] = 1;
    cache[2] = 3;
    cache[3] = 5;
    cache[4] = 10;
    cache_len = 5;
}

/* Ensure that the cache can hold indices up to max_idx (inclusive) */
static void ensure_capacity(idx_t max_idx)
{
    if (max_idx + 1 <= cache_len)
        return;

    idx_t new_len = cache_len ? cache_len << 1 : 1;
    while (new_len <= max_idx)
        new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return;                    /* allocation failure – nothing further */
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < 5)
        return cache[n];

    /* Start computing from the first missing index */
    idx_t start = cache_len;          /* first index not yet computed */
    ensure_capacity(n);

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t term = cache[i - 1] + 2 * cache[i - 2];
        term -= cache[i - 4];
        term -= cache[i - 5];
        cache[i] = term;
    }
    cache_len = n + 1;                /* new maximum index stored */

    return cache[n];
}
