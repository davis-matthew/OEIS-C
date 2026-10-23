/*
 * A117565  Numbers n such that n is 7*(the sum of decimal digits squared of
 *          n).
 * https://oeis.org/A117565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the n‑th integer (in increasing order) that satisfies * a
 *            = 7 × (sum of squares of its decimal digits). * This is a finite
 *            set – all solutions are ≤ 2268 – so we precompute * them once
 *            and cache the list for subsequent calls
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_square_sum.h"

static seq_t *cache = NULL;          /* array of found terms   */
static size_t cache_size = 0;        /* number of stored terms */
static size_t cache_capacity = 0;    /* array capacity         */
static idx_t last_checked = 0;       /* last integer examined  */
static bool initialized = false;    /* first‑time init flag   */

/* Grow the cache and add the new value. */
static void push_term(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – OEIS offset starts at 1 */

    /* Ensure the static cache is initialized exactly once. */
    if (!initialized) {
        push_term((seq_t)0);          /* 0 is always a solution */
        initialized = true;
    }

    /* Generate solutions up to the next needed index. */
    const idx_t LIMIT = 567 * 4;     /* 2268 – bound derived from n ≤ 567·d */
    while (cache_size < (size_t)n) {
        ++last_checked;
        if (last_checked > LIMIT)
            break;                   /* no further solutions possible */

        uint64_t d = digit_square_sum((uint64_t)last_checked);
        if ((seq_t)last_checked == (seq_t)(7 * d))
            push_term((seq_t)last_checked);
    }

    /* If we do not have enough terms, the requested n is beyond the
     * defined sequence.  Returning 0 conforms to the OEIS “b‑file”
     * convention for undefined entries. */
    if (cache_size < (size_t)n)
        return (seq_t)0;

    return cache[n - 1];
}
