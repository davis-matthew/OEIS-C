/*
 * A118536  Start with 1 and repeatedly reverse the digits and add 36 to get
 *          the next term.
 * https://oeis.org/A118536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; a(n)=reverse(a(n-1))+36 for n>1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 58 terms through n = 462 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* Undefined for n<1 */

    /* Memoisation cache: cache[i] = a(i+1) */
    static seq_t *cache = NULL;      /* dynamic array */
    static idx_t cache_size = 0;     /* number of computed terms */
    static size_t capacity = 0;      /* allocated capacity */

    if (cache_size < n) {
        /* Grow the allocated storage if necessary */
        size_t new_cap = (capacity == 0) ? 16 : capacity;
        while (new_cap < n) new_cap <<= 1;          /* double until enough */

        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                         /* out of memory */
        cache = tmp;
        capacity = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;               /* a(1) = 1 */
            } else {
                seq_t rev = reverse_digits(cache[i - 1]); /* reverse decimal digits */
                cache[i] = rev + (seq_t)36;        /* a(n) = rev(a(n-1)) + 36 */
            }
        }
        cache_size = n;
    }

    return cache[n - 1];                         /* a(n) */
}
