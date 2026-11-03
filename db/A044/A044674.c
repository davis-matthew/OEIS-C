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
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044674
   Numbers n such that the string "4,6" occurs in the base‑9 representation
   of n but not of n+1.  a(n) is the nth such integer (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑9 representation of x contains the substring 4,6. */
static bool has_46(seq_t x)
{
    if (x == 0)
        return false;

    /* Temporary storage for base‑9 digits (little‑endian). */
    unsigned char d[256];
    idx_t len = 0;
    while (x > 0) {
        d[len++] = (unsigned char)(x % 9);
        x = x / 9;
    }

    /* Look for the pattern 4,6 from most to least significant: in little‑endian
       it appears as 6 followed by 4. */
    for (idx_t i = 0; i + 1 < len; ++i)
        if (d[i] == 6 && d[i + 1] == 4)
            return true;
    return false;
}

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
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
