/*
 * A118154  Start with 1 and repeatedly reverse the digits and add 58 to get
 *          the next term.
 * https://oeis.org/A118154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n) = reverse_decimal(a(n-1)) + 58 for n>1 */
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;
    static size_t capacity = 0;

    if (n == 0)
        return (seq_t)0;               /* undefined; offset starts at 1 */

    /* Ensure cache can hold n terms */
    if (cached_size < n) {
        size_t needed = (size_t)n;
        if (capacity < needed) {
            size_t new_cap = capacity ? capacity * 2 : 1;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1); /* allocation failure */
            cache = tmp;
            capacity = new_cap;
        }

        /* Initialize first term if cache was empty */
        if (cached_size == 0) {
            cache[0] = (seq_t)1;
            cached_size = 1;
        }

        /* Compute forward from the last cached term */
        seq_t val = cache[cached_size - 1];
        for (idx_t i = cached_size; i < n; ++i) {
            val = reverse_decimal(val) + (seq_t)58;
            cache[i] = val;
        }
        cached_size = n;
    }

    return cache[n - 1];
}
