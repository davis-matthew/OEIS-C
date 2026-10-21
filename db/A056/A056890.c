/*
 * A056890  Denominators of continued fraction for left factorial.
 * https://oeis.org/A056890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(0)=1; a(1)=-1; a(2n)=n*a(2n-1)+a(2n-2); a(2n+1)=
 *            -a(2n)+a(2n-1)
 * Range:     i8 n <= 15, i16 n <= 21, i32 n <= 29, i64 n <= 45, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow non‑sequential calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* memory allocation failed – abort */
            abort();
        }
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)-1;
            } else if ((i & 1) == 0) {          /* i even  → 2n */
                idx_t m = i / 2;
                cache[i] = (seq_t)m * cache[i - 1] + cache[i - 2];
            } else {                           /* i odd   → 2n+1 */
                cache[i] = -cache[i - 1] + cache[i - 2];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
