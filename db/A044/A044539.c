/*
 * A044539  Numbers n such that string 2,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044539:
   Numbers n such that the base‑7 representation of n contains the
   substring "26", but the base‑7 representation of n+1 does not. */

/* The implementation builds the sequence on demand and caches it for
   subsequent calls.  The cache is extended only as far as needed to
   compute the requested index. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x (in base 7) contains the substring 2,6. */
static bool has26(seq_t x)
{
    if (x == 0)
        return false;
    unsigned prev = 255;               /* sentinel not in 0..6 */
    while (x > 0) {
        unsigned digit = (unsigned)(x % 7);
        if (prev == 6 && digit == 2)
            return true;
        prev = digit;
        x /= 7;
    }
    return false;
}

/* Return the n-th term (1-based) of sequence A044539. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 */

    static seq_t *cache = NULL;        /* cached terms */
    static size_t cache_len = 0;       /* number of cached terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static seq_t last_checked = 0;     /* largest n examined so far */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t candidate = last_checked + 1ul;

        /* Check the property for candidate and candidate+1. */
        if (has26(candidate) && !has26(candidate + 1)) {
            /* Ensure capacity. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) {            /* allocation failed */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        last_checked = candidate;
    }

    return cache[n - 1];
}
