/*
 * A171510  a(n) = 10*a(n-1) + a(n-2), with a(1)=2 and a(2)=1.
 * https://oeis.org/A171510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171510
   a(n) = 10*a(n-1) + a(n-2) ,  a(1)=2 , a(2)=1
*/
#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[1] = a(1), cache[2] = a(2) */
static idx_t  cache_len = 0;    /* largest index currently stored */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Allocate and initialise cache on first call. */
    if (cache == NULL)
    {
        size_t init_size = (n > 2 ? (size_t)n + 1 : 3); /* indices 0..n */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;                   /* out‑of‑memory guard */

        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache_len = 2;
    }

    /* Extend cache if necessary. */
    if (n > cache_len)
    {
        size_t new_size = (size_t)n + 1;                /* need 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* out‑of‑memory guard */
        cache = tmp;

        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            cache[i] = cache[i - 1] * (seq_t)10 + cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n];
}
