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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has_equal_19(seq_t value)
{
    unsigned count1 = 0, count9 = 0;
    while (value != 0)
    {
        int digit = (int)(value % 12);
        if (digit == 1) ++count1;
        else if (digit == 9) ++count9;
        value /= 12;
    }
    return count1 > 0 && count1 == count9;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;        /* allocation failure fallback */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = last_scanned;
        }
    }
    return cache[n - 1];
}
