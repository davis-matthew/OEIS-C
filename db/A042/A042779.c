/*
 * A042779  Denominators of continued fraction convergents to sqrt(920).
 * https://oeis.org/A042779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 182*a(n-2) - a(n-4) for n > 3 a(0) = 1, a(1) = 3, a(2) =
 *            181, a(3) = 546
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of a(0)..a(max_computed) */
    static idx_t cap   = 0;       /* currently allocated length */
    static idx_t stored = 0;      /* highest index for which a(i) is stored   */

    /* Ensure the cache can hold index n */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= n + 1) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – abort */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize the first four terms if necessary */
    if (stored < 4) {
        if (stored <= 0) cache[0] = (seq_t)1;
        if (stored <= 1) cache[1] = (seq_t)3;
        if (stored <= 2) cache[2] = (seq_t)181;
        if (stored <= 3) cache[3] = (seq_t)546;
        stored = 4;
    }

    /* Compute missing terms up to n */
    for (idx_t i = stored; i <= n; ++i) {
        if (i < 4) continue;              /* first four already set */
        seq_t temp = (seq_t)182 * cache[i - 2] - cache[i - 4];
        cache[i] = temp;
    }
    stored = n + 1;

    return cache[n];
}
