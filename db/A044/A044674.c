/*
 * A044674  Numbers n such that string 4,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_46
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044674
   Numbers n such that the string "4,6" occurs in the base‑9 representation
   of n but not of n+1.  a(n) is the nth such integer (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_46.h"

/* Main sequence function (public API) */
seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_size = 0;    /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t cur = 0;            /* current integer being examined */

    /* Generate new terms until we have at least n of them */
    if (n > cache_size) {
        for ( ; cache_size < n; ++cur ) {
            if (!has_46(cur))
                continue;                 /* no 46 in cur → cannot be a term */

            if (!has_46(cur + 1)) {       /* successor has no 46 → cur qualifies */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();            /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = cur;
            }
        }
    }

    return cache[n - 1];
}
