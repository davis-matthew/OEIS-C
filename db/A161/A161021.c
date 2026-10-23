/*
 * A161021  Collatz (or 3x+1) trajectory starting at 703.
 * https://oeis.org/A161021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 26, u32 n <= 170, u64 n <= 170, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A161021: Collatz (3x+1) trajectory starting at 703
   a(0) = 703
   a(n+1) = (a(n) is even) ? a(n)/2 : 3*a(n)+1
   After reaching 1, all subsequent terms are 1. */
#ifdef __cplusplus
  #define SEQ_FROM_INT(x) static_cast<seq_t>(x)
#else
  #define SEQ_FROM_INT(x) ((seq_t)(x))
#endif

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cached values stored in a static array */
    static seq_t *cache = NULL;
    static size_t size = 0;       /* number of elements already computed */
    static size_t cap  = 0;       /* allocated capacity */

    /* Ensure capacity is at least n+1 */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif
        if (!new_cache) abort();

        /* Copy existing values */
        for (size_t i = 0; i < size; ++i) {
            new_cache[i] = cache[i];
        }

        /* Release old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* Pre‑compute terms up to n */
    static const seq_t one   = SEQ_FROM_INT(1);
    static const seq_t two   = SEQ_FROM_INT(2);
    static const seq_t three = SEQ_FROM_INT(3);
    static const seq_t start = SEQ_FROM_INT(703);

    for (size_t i = size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = start;
        } else {
            const seq_t prev = cache[i-1];
            if (prev == one) {
                cache[i] = one;                /* freeze at 1 */
            } else if ((prev % two) == 0) {
                cache[i] = prev / two;
            } else {
                cache[i] = prev * three + one;
            }
        }
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
