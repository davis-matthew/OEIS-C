/*
 * A039388  Numbers whose base-9 representation has the same nonzero number of
 *          5's and 7's.
 * https://oeis.org/A039388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑9 representation contains the same
 *            positive number of 5's and 7's
 * Range:     u8 n <= 6, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cached sequence values */
    static size_t cache_size = 0;     /* number of terms cached */
    static size_t cache_capacity = 0; /* allocated capacity */
    static seq_t last_val = 0;        /* largest value already found */

    /* Handle non‑positive index (out of offset) */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        seq_t candidate;
        if (last_val < 52)
            candidate = 52;          /* first term is 52 */
        else
            candidate = last_val + 1;

        /* Find the next candidate that satisfies the property */
        for (;;) {
            seq_t tmp = candidate;
            unsigned count5 = 0, count7 = 0;

            /* Count 5's and 7's in base‑9 representation */
            do {
                int d = (int)(tmp % 9);
                if (d == 5) ++count5;
                else if (d == 7) ++count7;
                tmp /= 9;
            } while (tmp > 0);

            if (count5 == count7 && count5 > 0)
                break;   /* candidate satisfies condition */

            ++candidate;
        }

        /* Ensure there is space in the cache */
        if (cache_size >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return 0;        /* allocation failed */
            cache = new_cache;
            cache_capacity = new_cap;
        }

        cache[cache_size++] = candidate;
        last_val = candidate;
    }

    return cache[n - 1];
}
