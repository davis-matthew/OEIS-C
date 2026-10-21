/*
 * A209486  Number of 5-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6)
                 + a(n-8) - 2*a(n-10) + a(n-11)
   Initial values (n = 1 … 11):
     4, 38, 140, 390, 866, 1702, 3014, 4984, 7774, 11620, 16716
   Offset: 1
*/
#include <stdlib.h>

/* Memoization cache */
static seq_t *cache = NULL;          /* array of computed values, 0‑based index */
static size_t cache_size = 0;        /* allocated array size (in elements) */
static size_t cache_end   = 0;       /* largest index that has a value (>=0) */

/* Ensure the cache is large enough for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;

    /* grow the array, add a small extra room to reduce realloc frequency */
    size_t new_size = needed + 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();

    /* initialise new slots to zero */
    for (size_t i = cache_size; i < new_size; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_size = new_size;
}

/* initialise known values up to n = 11 */
static void init_base_values(void)
{
    if (cache_end >= 11) return;  /* already initialised */

    ensure_cache(11);
    cache[0] = 0;   /* unused, offset starts at 1 */
    cache[1] = 4;
    cache[2] = 38;
    cache[3] = 140;
    cache[4] = 390;
    cache[5] = 866;
    cache[6] = 1702;
    cache[7] = 3014;
    cache[8] = 4984;
    cache[9] = 7774;
    cache[10] = 11620;
    cache[11] = 16716;
    cache_end = 11;
}

/* Compute a(n) for any n >= 1.  Caller may call this in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, but return 0 */

    init_base_values();
    ensure_cache(n);

    /* compute missing values up to n */
    for (size_t i = cache_end + 1; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val = val - cache[i - 3];
        val = val - 2 * cache[i - 5];
        val = val + 2 * cache[i - 6];
        val = val + cache[i - 8];
        val = val - 2 * cache[i - 10];
        val = val + cache[i - 11];
        cache[i] = val;
    }
    cache_end = (size_t)n;   /* update last known index */

    return cache[(size_t)n];
}
