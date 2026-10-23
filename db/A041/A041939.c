/*
 * A041939  Denominators of continued fraction convergents to sqrt(492).
 * https://oeis.org/A041939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041939:  Denominators of continued fraction convergents to sqrt(492)
   Recurrence: a(n) = 59534*a(n-8) - a(n-16)  for n > 15
   Initial values (n = 0 .. 15):
   1, 5, 6, 11, 116, 127, 243, 1342,
   59291, 297797, 357088, 654885, 6905938, 7560823, 14466761, 79894628
   The function uses a simple memoisation cache to evaluate any index in
   O(n) time and O(n) memory. */

#include <stdlib.h>

static const seq_t _init[16] = {
    (seq_t)1,   (seq_t)5,   (seq_t)6,   (seq_t)11,
    (seq_t)116, (seq_t)127, (seq_t)243, (seq_t)1342,
    (seq_t)59291, (seq_t)297797, (seq_t)357088, (seq_t)654885,
    (seq_t)6905938, (seq_t)7560823, (seq_t)14466761, (seq_t)79894628
};

seq_t a(idx_t n)
{
    /* Static cache to memoise computed terms */
    static seq_t *cache = NULL;   /* dynamic array of terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* size of initialized prefix */

    /* Handle small indices directly */
    if (n < 16)
        return _init[(size_t)n];

    /* Ensure the cache is large enough for index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)  /* allocation failure; abort */
            exit(1);
        cache = new_cache;
        cap   = new_cap;

        /* First allocation: copy the 16 seed terms */
        if (len == 0) {
            for (size_t i = 0; i < 16; ++i)
                cache[i] = _init[i];
            len = 16;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        cache[i] = (seq_t)(59534) * cache[i - 8] - cache[i - 16];
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
