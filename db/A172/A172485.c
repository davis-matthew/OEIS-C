/*
 * A172485  The case S(-1,-2,3) of the family of self-convolutive recurrences
 *          studied by Martin and Kearney.
 * https://oeis.org/A172485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=1, a(2)=-1, a(3)=-1; a(n) = -3 * (a(n-1) + 2*a(n-2) +
 *            2*a(n-3)) for n > 3
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 68, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms stored */
    
    if (n == 0) return 0;  /* offset 1 – values below 1 are undefined; return 0 safely */

    if (n > cache_size) {
        idx_t new_size = (cache_size == 0) ? n : cache_size * 2;
        if (new_size < n) new_size = n;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – return 0, or could abort */
        cache = tmp;

        /* initialise the fixed first three terms if they weren't already set */
        if (new_size >= 1 && cache_size < 1) cache[0] = 1;       /* a(1) */
        if (new_size >= 2 && cache_size < 2) cache[1] = -1;     /* a(2) */
        if (new_size >= 3 && cache_size < 3) cache[2] = -1;     /* a(3) */

        /* compute terms from the old size up to new_size-1 */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            if (i < 3) continue;  /* first three terms already set */
            cache[i] = -3 * (cache[i - 1] + 2 * cache[i - 2] + 2 * cache[i - 3]);
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
