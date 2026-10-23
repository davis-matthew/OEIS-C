/*
 * A021537  Decimal expansion of 1/533.
 * https://oeis.org/A021537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^n / 533) mod 10, i.e. the n‑th decimal digit of
 *            1/533
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses long division with a cache so that calls in any order
   are handled in at most O(n) time. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *digits_cache = NULL;   /* array of cached digits */
static size_t cache_cap = 0;         /* allocated capacity of the array */
static size_t cache_size = 0;        /* number of digits actually computed */
static seq_t rem = 1;                /* current remainder < 533 */

seq_t a(idx_t n)
{
    /* Initialise on first use. */
    if (digits_cache == NULL) {
        size_t init_cap = 16;
        digits_cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (!digits_cache) return 0;          /* allocation failure */
        cache_cap = init_cap;
        cache_size = 0;
        rem = 1;
    }

    /* Ensure we have computed up to digit n. */
    if ((size_t)n >= cache_size) {
        size_t required = (size_t)n + 1;

        /* Grow the array if needed. */
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(digits_cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;                /* realloc failed */
            digits_cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute any missing digits by long division. */
        for (size_t i = cache_size; i < required; ++i) {
            unsigned int r10 = (unsigned int)rem * 10U;
            seq_t digit  = (seq_t)(r10 / 533U);
            unsigned int new_rem = r10 % 533U;
            digits_cache[i] = digit;
            rem = (seq_t)new_rem;
        }
        cache_size = required;
    }

    return digits_cache[(size_t)n];
}
