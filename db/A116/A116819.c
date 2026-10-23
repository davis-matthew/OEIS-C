/*
 * A116819  Number of permutations of length n which avoid the patterns 2431,
 *          4123, 4231.
 * https://oeis.org/A116819
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

/* Recurrence: a(1)=1, a(2)=2, a(3)=6, a(4)=21,
   a(n)=7*a(n-1) - 16*a(n-2) + 14*a(n-3) - 3*a(n-4)  (n≥5) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* maximum n value already computed */

    if (n == 0)                     /* offset starts at 1; return 0 for 0 */
        return (seq_t)0;

    if (n > cache_len) {
        /* Allocate or grow the cache to hold indices up to n. */
        size_t new_size = (size_t)(n + 1);        /* +1 because we use 1..n */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL)         /* allocation failed */
                return (seq_t)0;
            cache = tmp;
        }

        /* Compute new entries with the recurrence. */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k == 1)        cache[1] = (seq_t)1;
            else if (k == 2)   cache[2] = (seq_t)2;
            else if (k == 3)   cache[3] = (seq_t)6;
            else if (k == 4)   cache[4] = (seq_t)21;
            else {
                seq_t t1 = cache[k-1] * (seq_t)7;
                seq_t t2 = cache[k-3] * (seq_t)14;
                seq_t t3 = cache[k-2] * (seq_t)16;
                seq_t t4 = cache[k-4] * (seq_t)3;
                cache[k] = (t1 + t2) - (t3 + t4);
            }
        }
        cache_len = n;
    }
    return cache[n];
}
