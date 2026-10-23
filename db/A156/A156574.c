/*
 * A156574  a(n) = 34*a(n-1) - a(n-2) - 4232 for n > 2; a(1)=1369, a(2)=42025.
 * https://oeis.org/A156574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) - 4232 for n>2; a(1)=1369, a(2)=42025
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* number of terms already cached */

    if (n == 0)
        return 0;                     /* out of bounds, unused for OEIS */

    /* extend cache if required */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)              /* allocation failed */
            return 0;
        cache = tmp;

        /* initial values */
        if (cache_len == 0) {
            cache[0] = (seq_t)1369;
            cache_len = 1;
        }
        if (cache_len == 1 && new_len >= 2) {
            cache[1] = (seq_t)42025;
            cache_len = 2;
        }

        /* compute remaining terms with the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)34 * cache[i-1] - cache[i-2] - (seq_t)4232;
        }
        cache_len = new_len;
    }

    return cache[n-1];                  /* OEIS indexing starts at 1 */
}
