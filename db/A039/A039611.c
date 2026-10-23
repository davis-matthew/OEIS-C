/*
 * A039611  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 9's.
 * https://oeis.org/A039611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-12 representation has the same nonzero number
 *            of 1's and 9's
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_19.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, guard */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last_scanned = 0;

    while (len < n)
    {
        last_scanned++;
        if (has_equal_19(last_scanned))
        {
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failure fallback */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = last_scanned;
        }
    }
    return cache[n - 1];
}
