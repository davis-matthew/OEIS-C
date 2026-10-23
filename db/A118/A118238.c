/*
 * A118238  Start with 15 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   reverse_decimal, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A118238
 * a(1) = 15
 * a(n+1) = reverse_decimal(a(n)) + 1   for n >= 1
 * (reverse_decimal reverses the decimal digits of its argument.)
 */

#include <stddef.h>
#include "../utils/reverse_decimal.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Memoised generation of A118238 */
seq_t a(idx_t n)
{
    if (n == 0) {          /* unsupported offset; return 0 */
        return (seq_t)0;
    }

    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static idx_t cache_len = 0;   /* number of terms stored */

    /* enlarge cache if necessary */
    if (cache_len < n) {
        /* allocate a new array large enough for all requested terms */
        size_t needed = (size_t)n;
        seq_t *new_cache = seq_alloc_array(needed);
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        /* initialise the missing terms */
        seq_t prev;
        size_t start_index;
        if (cache_len == 0) {
            new_cache[0] = (seq_t)15;   /* a(1) */
            prev = new_cache[0];
            start_index = 1;
        } else {
            prev = cache[cache_len - 1];
            start_index = (size_t)cache_len;
        }
        for (size_t i = start_index; i < needed; ++i) {
            seq_t next = reverse_decimal(prev) + (seq_t)1;
            new_cache[i] = next;
            prev = next;
        }

        /* free the old cache and replace it */
        seq_free_array(cache);
        cache = new_cache;
        cache_len = (idx_t)needed;
    }

    /* cache now contains at least n terms */
    return cache[n - 1];
}
