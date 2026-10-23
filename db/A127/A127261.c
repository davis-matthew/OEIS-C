/*
 * A127261  a(n) = 2*a(n-1) + 10*a(n-2), with a(0) = 2 and a(1) = 2.
 * https://oeis.org/A127261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n‑1) + 10*a(n‑2) (a(0)=2, a(1)=2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A127261. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cap       = 0;        /* allocated capacity */
    static idx_t  len       = 0;        /* number of terms stored */

    /* Handle the first two terms directly. */
    if (n == 0u || n == 1u) {
        return (seq_t)2;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1u > cap) {
        size_t new_cap = (size_t)n + 1u;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In an evaluative environment abort is acceptable. */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base cases if they aren't already stored. */
    if (len == 0) {
        cache[0] = (seq_t)2;
        len = 1;
    }
    if (len < 2) {
        cache[1] = (seq_t)2;
        len = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)10 * cache[i - 2];
    }
    len = n + 1u;  /* update stored length */

    return cache[n];
}
