/*
 * A203366  Number of n X 3 0..2 arrays with every 1 immediately preceded by 0
 *          to the left or above, and every 2 immediately preceded by both a 1
 *          and a 0.
 * https://oeis.org/A203366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A203366
   Recurrence:
     a(n) = 3*a(n-1) + 26*a(n-2) + 44*a(n-3) + 45*a(n-4) + 24*a(n-5)
            - a(n-6) - 17*a(n-7) + 2*a(n-8) - 2*a(n-9)
   with initial terms
     a(1)=3, a(2)=30, a(3)=206, a(4)=1570, a(5)=11541,
     a(6)=85932, a(7)=636912, a(8)=4728287, a(9)=35081894.
   The sequence is memoised; calls may be in any order.
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;         /* cache[n-1] holds a(n) */
static size_t cache_cap = 0;        /* allocated capacity of cache */
static size_t cache_len = 0;        /* number of terms currently cached */

/* Ensure that cache has at least 'needed' slots allocated. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory: stop execution */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS offset; for safety handle n==0 */
    if (n == 0) return (seq_t)0;

    /* Convert n to size_t safely for array indexing (n <= 10000 in tests) */
    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    ensure_capacity(idx);

    /* Initialise cache with first nine terms on the first call */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)206;
        cache[3] = (seq_t)1570;
        cache[4] = (seq_t)11541;
        cache[5] = (seq_t)85932;
        cache[6] = (seq_t)636912;
        cache[7] = (seq_t)4728287;
        cache[8] = (seq_t)35081894;
        cache_len = 9;
    }

    /* Return already computed term */
    if (idx <= cache_len) return cache[idx - 1];

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i < idx; ++i)
    {
        seq_t term =
            (seq_t)3  * cache[i - 1] +
            (seq_t)26 * cache[i - 2] +
            (seq_t)44 * cache[i - 3] +
            (seq_t)45 * cache[i - 4] +
            (seq_t)24 * cache[i - 5] -
            cache[i - 6] -
            (seq_t)17 * cache[i - 7] +
            (seq_t)2  * cache[i - 8] -
            (seq_t)2  * cache[i - 9];
        cache[i] = term;
    }

    cache_len = idx;
    return cache[idx - 1];
}
