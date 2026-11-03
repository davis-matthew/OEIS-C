/*
 * A118526  Start with 1 and repeatedly reverse the digits and add 7 to get
 *          the next term.
 * https://oeis.org/A118526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 7, with a(1) = 1
 * Range:     u8 n <= 13, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Initialize first element if needed. */
    if (len == 0) {
        cache = mem_new(seq_t, 1);
        cap   = 1;
        len   = 1;
        cache[0] = (seq_t)1;
    }

    if ((idx_t)len < n) {
        /* Grow capacity to at least n. */
        size_t new_cap = cap;
        while ((idx_t)new_cap < n) {
            new_cap = new_cap ? new_cap * 2 : 1;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cap   = new_cap;

        /* Compute missing terms. */
        for (size_t i = len; i < n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = reverse_digits(prev) + (seq_t)7;
        }
        len = n;
    }

    return cache[n - 1];
}
