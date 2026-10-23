/*
 * A199804  G.f.: 1/(1+x+x^3).
 * https://oeis.org/A199804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) - a(n-3), a(0)=1, a(1)=-1, a(2)=1 (derived from
 *            the generating function 1/(1+x+x^3) )
 * Range:     i8 n <= 13, i16 n <= 28, i32 n <= 57, i64 n <= 115,
 *            i128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms       */
    static size_t cache_sz = 0;   /* number of terms currently stored   */
    const size_t need = (size_t)n + 1;  /* we need indices 0 .. n inclusive */

    if (need > cache_sz) {              /* expand cache if necessary           */
        const size_t old_sz = cache_sz;  /* previous size */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory: safe fallback       */
        cache = tmp;

        /* compute all new entries from old_sz up to n */
        for (size_t i = old_sz; i < need; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)-1;
            } else if (i == 2) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = -cache[i-1] - cache[i-3];
            }
        }
        cache_sz = need;
    }
    return cache[(size_t)n];
}
