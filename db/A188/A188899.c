/*
 * A188899  Third row of array in A187617.
 * https://oeis.org/A188899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 25*a(n-2) + 11*a(n-3) - a(n-4), with a(0)=1,
 *            a(1)=5, a(2)=36, a(3)=281. This follows from the generating
 *            function (1-x)*(x^2-5*x+1)/(x^4-11*x^3+25*x^2-11*x+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static size_t cached_len = 0;  /* number of values actually stored */

    /* ensure cache has room for indices up to n */
    size_t idx = (size_t)n;
    if (idx + 1 > cache_cap) {
        size_t new_cap = idx + 1;
        /* enlarge by at least the needed size */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* base values for n = 0..3 */
    const seq_t base[4] = { (seq_t)1, (seq_t)5, (seq_t)36, (seq_t)281 };

    /* if we are still before the first stored values, ensure they exist */
    if (cached_len < 4 && cached_len <= idx) {
        size_t upto = (idx < 3 ? idx : 3);
        for (size_t i = cached_len; i <= upto; ++i) {
            cache[i] = base[i];
        }
        cached_len = upto + 1;
    }

    /* compute any missing terms using the recurrence */
    for (size_t i = cached_len; i <= idx; ++i) {
        /* recurrence uses indices i-1 .. i-4
           existing base terms will be already set for i<4 */
        cache[i] = (  ( (seq_t)11 * cache[i-1])
                    - ( (seq_t)25 * cache[i-2])
                    + ( (seq_t)11 * cache[i-3])
                    -           cache[i-4]);
    }
    cached_len = idx + 1;

    return cache[idx];
}
