/*
 * A122605  Expansion of -x*(2*x - 1)*(2*x^2 - 1)*(x^3 + 2*x^2 - x - 1)/((x -
 *          1)*(x^2 + x - 1)*(x^4 - 4*x^3 - 4*x^2 + x + 1)).
 * https://oeis.org/A122605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   A122605 a(1) = 1; a(2)…a(7) = 0; for n ≥ 8 a(n) =
 *            a(n-1)+6*a(n-2)-5*a(n-3)-10*a(n-4)+6*a(n-5)+4*a(n-6)-a(n-7)
 * Range:     i8 n <= 13, i16 n <= 21, i32 n <= 38, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – behaviour not required */

    static seq_t *cache   = NULL;         /* stored terms, cache[i] is a(i+1) */
    static size_t cap     = 0;            /* allocated size of cache       */
    static size_t last    = (size_t)-1;   /* highest index with a value    */

    const size_t idx_needed = (size_t)(n - 1);   /* internal 0‑based index      */

    /* enlarge allocation if required */
    if (idx_needed + 1 > cap) {
        size_t newcap = idx_needed + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failed */
        cache = tmp;
        /* initialise new cells to 0 (prevent garbage) */
        for (size_t i = cap; i < newcap; ++i) cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* initialise base values on first use */
    if (last == (size_t)-1) {
        cache[0] = (seq_t)1;                      /* a(1) */
        for (size_t i = 1; i <= 6 && i < cap; ++i)
            cache[i] = (seq_t)0;                  /* a(2)…a(7) = 0 */
        last = (cap > 6 ? 6 : cap - 1);
    }

    /* compute any missing values up to idx_needed */
    if (idx_needed > last) {
        for (size_t i = last + 1; i <= idx_needed; ++i) {
            if (i <= 6) {                         /* safety for small i */
                cache[i] = (seq_t)0;
                continue;
            }
            seq_t val = cache[i - 1];
            val += (seq_t)6  * cache[i - 2];
            val -= (seq_t)5  * cache[i - 3];
            val -= (seq_t)10 * cache[i - 4];
            val += (seq_t)6  * cache[i - 5];
            val += (seq_t)4  * cache[i - 6];
            val -=          cache[i - 7];
            cache[i] = val;
        }
        last = idx_needed;
    }

    return cache[idx_needed];
}
