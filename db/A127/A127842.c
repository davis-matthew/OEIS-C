/*
 * A127842  a(1)=1, a(2)=...=a(8)=0, a(n) = a(n-8)+a(n-7) for n>8.
 * https://oeis.org/A127842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 149, u32 n <= 269, u64 n <= 509,
 *            u128 n <= 989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=…=a(8)=0, a(n)=a(n-8)+a(n-7) for n>8 */
#include <stdlib.h>

/* Cache the sequence values to allow arbitrary call order */
static seq_t *cache = NULL;
static idx_t cached_n = 0;

seq_t a(idx_t n)
{
    /* undefined behaviour for n==0, return 0 */
    if (n == 0) return 0;

    /* initialise cache once */
    if (cache == NULL) {
        size_t init_size = (size_t)(8 > n ? 8 : n) + 1;  /* indices 0..max(n,8) */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0;                         /* unlikely to fail */

        /* basic terms */
        cache[1] = 1;
        for (idx_t i = 2; i <= 8; ++i) cache[i] = 0;
        cached_n = 8;
    }

    /* already computed up to n?  */
    if (n <= cached_n) return cache[n];

    /* grow array to accommodate n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                         /* allocation failure */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = cache[i - 8] + cache[i - 7];
    }

    cached_n = n;
    return cache[n];
}
