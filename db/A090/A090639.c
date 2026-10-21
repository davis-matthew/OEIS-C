/*
 * A090639  a(0) = 0; a(2n) = 3*a(n), a(2n+1) = a(n) + 1.
 * https://oeis.org/A090639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(2n)=3*a(n); a(2n+1)=a(n)+1 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t  cache_size = 0;    /* current size of the cache */

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size << 1 : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_cache(n);

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Recursive computation using the recurrence */
    seq_t res;
    if ((n & 1) == 0) {          /* even: a(2n) = 3*a(n) */
        res = 3 * a(n >> 1);
    } else {                     /* odd:  a(2n+1) = a(n) + 1 */
        res = a(n >> 1) + 1;
    }
    cache[n] = res;
    return res;
}
