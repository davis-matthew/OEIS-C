/*
 * A021497  Decimal expansion of 1/493.
 * https://oeis.org/A021497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the nth digit after the decimal point in the decimal
 * expansion of 1/493.  So a(0)=0, a(1)=0, a(2)=2, …
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored digits            */
    static size_t cache_len = 0;         /* number of stored digits  */
    static size_t cache_cap = 0;         /* allocated capacity       */
    static uint64_t rem = 1;             /* current remainder        */

    /* fast return if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* initialise storage on first use */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow allocation if necessary */
    if (cache_len + (size_t)(n - cache_len + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < cache_len + (size_t)(n - cache_len + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failure – unlikely for these limits */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing digits by long division */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        rem *= 10ULL;                 /* bring down next zero   */
        uint64_t d = rem / 493ULL;    /* next digit             */
        rem %= 493ULL;                /* remainder for next step*/
        cache[i] = (seq_t)d;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
