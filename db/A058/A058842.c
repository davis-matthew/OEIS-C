/*
 * A058842  From Renyi's "beta expansion of 1 in base 3/2": sequence gives
 *          a(1), a(2), ... where x(n) = a(n)/2^n, with 0 < a(n) < 2^n, a(1) =
 *          1, a(n) = 3*a(n-1) modulo 2^n.
 * https://oeis.org/A058842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3*a(n-1) mod 2^n with a(1) = 1 The sequence is computed
 *            iteratively with memoisation
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of computed terms
   cache[0] holds a(1), cache[1] holds a(2), etc.
*/
static seq_t *cache = NULL;          /* dynamic array of terms   */
static size_t cache_len = 0;         /* number of terms in cache */

/* compute term a(n)   (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, but harmless */

#if 0
    /* The following guard keeps the code safe when the native type
       has less than 64 bits.  For larger native types or for
       arbitrary‑precision types (which have an undefined sizeof)
       the mask is built by shifting 1 by n bits. */

    if (n <= cache_len)
        return cache[n-1];
#endif

    /* Expand cache up to n */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)                    /* OOM – return 0 as a safe value */
            return 0;
        cache = tmp;
        if (cache_len == 0) {
            cache[0] = (seq_t)1;    /* a(1) = 1  */
            cache_len = 1;
        }
        /* compute missing terms */
        for (size_t i = cache_len + 1; i <= new_len; ++i) {
            seq_t prev = cache[i-2];          /* a(i-1) */
            seq_t val  = prev * (seq_t)3;     /* 3*a(i-1) */

            /* build mask = 2^i – 1, taking into account
               that shifting beyond the native word size is UB. */
            seq_t mask;
            if (sizeof(seq_t) <= 8) {          /* 8‑byte or smaller types */
                size_t width = 8 * sizeof(seq_t);  /* max bits representable */
                if (i < width)
                    mask = ((seq_t)1 << i) - (seq_t)1;
                else
                    mask = ~(seq_t)0;          /* all bits set, no further reduction */
            } else {                           /* arbitrary‑precision, arbitrary size */
                mask = ((seq_t)1 << i) - (seq_t)1;
            }

            cache[i-1] = val & mask;
        }
        cache_len = new_len;
    }
    return cache[n-1];
}
