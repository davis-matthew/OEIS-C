/*
 * A138484  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 0.
 * https://oeis.org/A138484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   term_from_prev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0) = 0;
 *             For n > 0, a(n) is obtained from a(n-1) by
 *             reading the digits of a(n-1) from right to left,
 *             and for each distinct digit d encountered,
 *             appending the decimal representation of the
 *             total count of d in a(n-1) followed by d.
 *
 * This implements the OEIS sequence A138484.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/term_from_prev.h"

/* Cache for computed values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache contains all terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);                     /* out of memory */

    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = 0;
        else
            cache[i] = term_from_prev(cache[i - 1]);
    }
    cache_len = new_len;
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
