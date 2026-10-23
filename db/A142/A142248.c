/*
 * A142248  Odd numbers in A138123.
 * https://oeis.org/A142248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A142248: Odd numbers in A138123
    Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3) - a(n-4) - a(n-5)   for n > 6
    Initial terms: a(1)=1, a(2)=3, a(3)=7, a(4)=11, a(5)=17, a(6)=29
    All other values are defined by the above recurrence.  The function
    computes a(n) for any n ≥ 1, memoizing results to handle arbitrary
    call order efficiently.  It works with any unsigned integer type
    (including arbitrary-precision classes) provided the usual arithmetic
    operators are implemented.  The implementation uses the standard
    library's malloc/realloc for dynamic storage.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and bookkeeping */
    static seq_t *cache = NULL;                 /* array of values, 1‑indexed */
    static idx_t   cache_size = 0;              /* allocated size (>= 0) */
    static idx_t   last_computed = 0;           /* largest n computed so far */

    /* Handle initial terms directly */
    if (n <= 6) {
        static const seq_t init[] = {0, 1, 3, 7, 11, 17, 29};  /* init[0] unused */
        return init[n];
    }

    /* Ensure cache is large enough: at least n+1 elements, index 1‑based */
    if (n > cache_size) {
        /* Increase capacity (double or set to n, whichever is bigger) */
        idx_t new_size = (n > 2 * cache_size) ? n : 2 * cache_size;
        /* For safety, guarantee at least 6 elements for the base cases */
        if (new_size < 6) new_size = 6;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache) return 0;                     /* allocation failure */
        cache = new_cache;
        cache_size = new_size;

        /* (Re)initialize base terms if not already present */
        if (last_computed < 6) {
            cache[1] = 1;
            cache[2] = 3;
            cache[3] = 7;
            cache[4] = 11;
            cache[5] = 17;
            cache[6] = 29;
            last_computed = 6;
        }
    }

    /* Compute any missing terms using the recurrence */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                       - cache[i - 4] - cache[i - 5];
        }
        last_computed = n;
    }

    return cache[n];
}
