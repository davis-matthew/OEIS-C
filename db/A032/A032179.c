/*
 * A032179  Number of necklaces with n labeled beads of 3 colors.
 * https://oeis.org/A032179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * (n-1)! (equivalently a(n) = 3*(n-1)*a(n-1), a(1)=3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)  /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)3;
            } else {
                seq_t factor = (seq_t)3 * (seq_t)(i - 1);
                cache[i - 1] = cache[i - 2] * factor;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
