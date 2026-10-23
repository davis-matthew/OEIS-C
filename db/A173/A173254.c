/*
 * A173254  a(n) = a(n-1) + a(n-2) - [a(n-2)/2] - [a(n-4)/2].
 * https://oeis.org/A173254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - [a(n-2)/2] - [a(n-4)/2] a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 44, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- static cache ------------------------------------------- */
    static seq_t *cache   = NULL;   /* array of already computed terms   */
    static size_t cache_cap = 0;    /* allocated capacity (in terms)      */
    static size_t cache_size = 0;   /* number of terms stored, last valid index +1 */

    /* --- lazy initialization ------------------------------------- */
    if (cache == NULL) {
        /* start with capacity for 16 terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);

        /* set the first five terms supplied by OEIS */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;

        cache_size = 5;  /* indices 0..4 are now known */
    }

    /* --- ensure we have enough memory for n + 1 terms ----------- */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute any missing terms up to n --------------------- */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i < 5) {            /* values already present for i<5 */
            continue;           /* no computation needed */
        }
        seq_t term = cache[i-1] + cache[i-2];
        term -= (cache[i-2] >> 1); /* floor division by 2 */
        term -= (cache[i-4] >> 1); /* floor division by 2 */
        cache[i] = term;
    }

    /* --- update cached size (never shrink) --------------------- */
    if ((size_t)(n + 1) > cache_size) {
        cache_size = (size_t)(n + 1);
    }

    return cache[n];
}
