/*
 * A118535  Start with 1 and repeatedly reverse the digits and add 20 to get
 *          the next term.
 * https://oeis.org/A118535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=reverse_decimal(a(n-1))+20 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cached = 0;              /* number of terms stored */

    /* Grow the cache if necessary */
    if (cached < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed */
        cache = new_cache;
        /* cached remains unchanged – we will fill new entries below */
    }

    /* Initialize the first term if not already */
    if (cached == 0) {
        cache[0] = (seq_t)1;
        cached = 1;
    }

    /* Compute any missing terms */
    for (idx_t i = cached; i < n; ++i) {
        seq_t rev = reverse_decimal(cache[i - 1]);
        cache[i] = rev + (seq_t)20;
    }
    cached = n;   /* update count of stored terms */

    return cache[n - 1];
}
