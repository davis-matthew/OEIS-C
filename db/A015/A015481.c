/*
 * A015481  q-Fibonacci numbers for q=9, scaling a(n-1).
 * https://oeis.org/A015481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n-1)*a(n-1) + a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_cap = 0;         /* capacity of the array           */
static size_t cache_len = 0;         /* number of valid entries         */
static seq_t pow9 = 1;               /* 9^(i-1) for the next term i     */

seq_t a(idx_t n)
{
    /* Ensure array capacity for indices 0..n */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1 > 2 ? n + 1 : 2);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;                /* entries 0 and 1 are ready */
    } else if ((size_t)n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache     = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Compute missing values up to n */
    if (n >= (idx_t)cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            pow9  *= (seq_t)9;                 /* 9^(i-1)  */
            cache[i] = pow9 * cache[i - 1] + cache[i - 2];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
