/*
 * A127009  a(1)=1. a(n) = the numerator of the sum of the reciprocals of the
 *          earlier terms of the sequence which are coprime to n.
 * https://oeis.org/A127009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1, a(n) is the numerator of the reduced sum
 *            \sum_{k=1}^{n-1, gcd(a(k),n)=1} 1/a(k)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* 0‑indexed array of computed terms     */
static size_t cache_len = 0;     /* number of terms currently stored      */

/* Return a(n) for every n >= 1, in any order of calls. */
seq_t a(idx_t n)
{
    if (n < 1)            /* Undefined behaviour for n==0, but never called */
        return 0;

    /* Grow the cache if needed. */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)           /* Memory exhaustion – abort */
            exit(EXIT_FAILURE);
        cache = new_cache;

        /* Compute missing terms up to n. */
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t k = (idx_t)(i + 1);   /* 1‑based index of the new term */

            if (k == 1) {
                cache[i] = 1;          /* a(1) = 1 */
                continue;
            }

            /* Reduced fraction for the sum of reciprocals of earlier
               terms that are coprime to k. */
            seq_t num = 0;   /* numerator   */
            seq_t den = 1;   /* denominator */

            for (size_t j = 0; j < i; ++j) {
                if (gcd(cache[j], (seq_t)k) == 1) {
                    seq_t t = cache[j];
                    seq_t g = gcd(den, t);

                    seq_t den_div_g = den / g;
                    seq_t t_div_g   = t   / g;

                    /* num/den + 1/t  */
                    seq_t new_num = num * t_div_g + den_div_g;
                    seq_t new_den = den * t_div_g;

                    seq_t gg = gcd(new_num, new_den);
                    num = new_num / gg;
                    den = new_den / gg;
                }
            }
            cache[i] = num;   /* store the numerator of the reduced sum */
        }
        cache_len = new_len;
    }

    /* Return requested term (cache is 0‑based). */
    return cache[n - 1];
}
