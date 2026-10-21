/*
 * A052341  Shifts left two places under BIN1 transform.
 * https://oeis.org/A052341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052341:
   a(0)=a(1)=a(2)=1, a(3)=-3, a(4)=7
   for n>=5   a(n) = -3*a(n-1)-3*a(n-2)-a(n-3)+a(n-4)+a(n-5)
*/
#include <stdlib.h>
#include <stddef.h>
static seq_t *cache = NULL;      /* cached values a(0)..a(k) */
static size_t  cache_size = 0;   /* current number of cached values */

seq_t a(idx_t n)
{
    /* trivial small indices */
    if (n == 0 || n == 1 || n == 2) return 1;
    if (n == 3) return -3;
    if (n == 4) return 7;

    /* ensure cache holds at least first 5 terms */
    if (cache_size < 5) {
        size_t new_size = 5;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cache[0] = cache[1] = cache[2] = 1;
        cache[3] = -3;
        cache[4] = 7;
        cache_size = new_size;
    }

    /* enlarge cache up to n+1 if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i <= n; ++i) {
            cache[i] = - (seq_t)3 * cache[i-1]
                     - (seq_t)3 * cache[i-2]
                     - (seq_t)1 * cache[i-3]
                      + cache[i-4]
                      + cache[i-5];
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
