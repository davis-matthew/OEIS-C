/*
 * A160844  Number of lines through at least 2 points of a 4 X n grid of
 *          points.
 * https://oeis.org/A160844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=18, a(3)=35 a(n) = 2*a(n-1) - a(n-2) +
 *            C[(n+2)%6] , n >= 4 where C = {10,4,12,2,12,4} (the second
 *            recurrence a(n)=a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) gives the
 *            same values) The function caches all computed values in a static
 *            array so that repeated calls, even in arbitrary orde...
 * Range:     u8 n <= 8, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */
#include <stddef.h>          /* size_t */

static seq_t *a_cache = NULL;        /* array of computed terms */
static size_t  cache_cap = 0;        /* allocated size of the array   */
static idx_t   cache_max = 0;        /* largest index computed so far */

/* periodic constants used in the recurrence */
static const seq_t C[6] = {10, 4, 12, 2, 12, 4};

seq_t a(idx_t n)
{
    /* Initialisation of the cache and first four terms */
    if (a_cache == NULL) {
        cache_cap = 8;                         /* initial capacity */
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (a_cache == NULL) return 0;         /* allocation failure */
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)18;
        a_cache[3] = (seq_t)35;
        cache_max = 3;                         /* indices 0..3 are known */
    }

    /* Fast return if the requested value is already cached */
    if (n <= cache_max)
        return a_cache[n];

    /* Ensure enough space for indices up to n */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure */
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    idx_t i;
    for (i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0)        val = (seq_t)0;
        else if (i == 1)   val = (seq_t)1;
        else if (i == 2)   val = (seq_t)18;
        else if (i == 3)   val = (seq_t)35;
        else {
            /*  a(i) = 2*a(i-1) - a(i-2) + C[(i+2)%6]  */
            val = (seq_t)(2 * a_cache[i - 1] - a_cache[i - 2] + C[(i + 2) % 6]);
        }
        a_cache[i] = val;
    }
    cache_max = n;
    return a_cache[n];
}
