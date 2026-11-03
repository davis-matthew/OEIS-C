/*
 * A021856  Decimal expansion of 1/852.
 * https://oeis.org/A021856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of 1/852 at position n (digits after
 *            decimal point)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t rem = 1;           /* remainder before extracting the current digit */
    static bool init = false;

    if (!init) {
        /* allocate initial cache */
        cap = 128;
        cache = mem_new(seq_t, cap);
        init = true;
    }

    /* compute digits up to index n, caching them */
    while (len <= n) {
        rem *= (seq_t)10;                 /* bring down next decimal place */
        seq_t d = rem / (seq_t)852;        /* next digit */
        rem = rem % (seq_t)852;            /* new remainder */

        if (len >= cap) {                 /* grow cache if needed */
            cap <<= 1;
            cache = mem_resize(seq_t, cache, cap);
        }
        cache[len++] = d;
    }

    return cache[(size_t)n];
}
