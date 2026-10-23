/*
 * A116738  Number of permutations of length n which avoid the patterns 3214,
 *          4123, 4132.
 * https://oeis.org/A116738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2)=2; a(3)=6;
   a(n) = a(n-1)*4 - a(n-2)*2 + a(n-3)  for n >= 4           */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;     /* cache[0] = a(1), cache[1] = a(2), ...   */
static size_t cache_len = 0;    /* number of elements currently cached   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 simply */

    /* enlarge cache if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            abort(); /* memory allocation failure */
        }
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1)
                cache[0] = (seq_t)1;
            else if (i == 2)
                cache[1] = (seq_t)2;
            else if (i == 3)
                cache[2] = (seq_t)6;
            else {
                /* a(i) = 4*a(i-1) - 2*a(i-2) + a(i-3)           */
                seq_t val = cache[i - 2] * (seq_t)4;
                val -= cache[i - 3] * (seq_t)2;
                val += cache[i - 4];
                cache[i - 1] = val;
            }
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
