/*
 * A084178  Inverse binomial transform of Fibonacci oblongs.
 * https://oeis.org/A084178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A084178 – Inverse binomial transform of Fibonacci oblongs.
   Recurrence:  a(0)=0, a(1)=1, a(2)=0
                a(n) = -a(n-1) + 3*a(n-2) + 2*a(n-3)   for n≥3
*/

/* use standard integer types */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build; do NOT redefine them. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache for first three terms on first call */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;        /* insufficient memory – fallback */
        cache[0] = 0;                /* a(0) */
        cache[1] = 1;                /* a(1) */
        cache[2] = 0;                /* a(2) */
    }

    /* if requested term not yet computed, extend cache */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;           /* memory fault – safe return */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = -a(i-1) + 3*a(i-2) + 2*a(i-3) */
            cache[i] = -cache[i - 1] + (seq_t)3 * cache[i - 2] + (seq_t)2 * cache[i - 3];
        }
        cache_len = new_len;
    }

    return cache[n];
}
