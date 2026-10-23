/*
 * A118218  Start with 1 and repeatedly reverse the digits and add 71 to get
 *          the next term.
 * https://oeis.org/A118218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_digits_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n) = reverse(a(n-1)) + 71 for n > 1 */
#include <stdlib.h>
#include "../utils/reverse_digits_local.h"

/* Dynamic cache for the sequence values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* current allocated capacity */
static idx_t computed = 0;    /* number of terms stored in cache */

/* Ensure that the cache can hold at least n elements (0‑based). */
static void ensure_size(idx_t n)
{
    if (cache_cap >= n)
        return;
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    ensure_size(n);          /* need space for indices 0 .. n-1 */

    /* Initialize cache with the first term if we haven't yet. */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_digits_local(prev);
        seq_t cur  = rev + (seq_t)71;
        cache[i] = cur;
    }

    computed = n;
    return cache[n - 1];
}
