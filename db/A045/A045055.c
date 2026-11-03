/*
 * A045055  Numbers whose base-4 representation contains exactly three 0's and
 *          three 2's.
 * https://oeis.org/A045055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045055 - Numbers whose base-4 representation contains exactly
   three 0's and three 2's.  Computed by brute‑force enumeration. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: true if x has exactly three 0's and exactly three 2's in base‑4. */
static bool has_three_zeros_and_three_twos(seq_t x)
{
    unsigned count0 = 0, count2 = 0;
    /* Empty representation is not needed (x > 0 when called). */
    while (x != (seq_t)0) {
        unsigned digit = (unsigned)(x % (seq_t)4);
        if (digit == 0) {
            if (++count0 > 3) return false;
        } else if (digit == 2) {
            if (++count2 > 3) return false;
        }
        x /= (seq_t)4;
    }
    return (count0 == 3) && (count2 == 3);
}

/* Dyn‑array of computed values. */
static seq_t *cache = NULL;          /* pointer to array of terms */
static size_t  cache_len = 0;        /* number of terms currently stored */
static size_t  cache_cap = 0;        /* allocated capacity of the array */
static seq_t  next_candidate = (seq_t)0; /* value to start scanning from */

/* Ensure the cache has room for at least ‘needed’ elements. */
static void ensure_capacity(size_t needed)
{
    if (cache == NULL || cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  Behaviour for n < 1 is undefined (but we protect anyway). */
    if (n == 0) return (seq_t)0;

    /* Compute all terms up to n on demand. */
    while (cache_len < n) {
        /* Find next number that satisfies the property. */
        do {
            ++next_candidate;
        } while (!has_three_zeros_and_three_twos(next_candidate));

        /* Store the term. */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = next_candidate;
    }
    /* Return the requested term (offset 1). */
    return cache[n - 1];
}
