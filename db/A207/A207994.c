/*
 * A207994  Number of nX5 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor (colorings ignoring permutations of colors).
 * https://oeis.org/A207994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 65*a(n-2) + 92*a(n-3) - 48*a(n-4) + 8*a(n-5),
 *            a(1) = 8, a(2) = 81, a(3) = 853, a(4) = 9075, a(5) = 96831
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;          /* number of terms stored (1‑based) */

    if (n == 0) return (seq_t)0;      /* undefined for n=0, harmless */

    /* Initialise cache with the first five terms on the first call */
    if (len == 0) {
        const size_t init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)853;
        cache[3] = (seq_t)9075;
        cache[4] = (seq_t)96831;
        len    = 5;
        cap    = init_cap;
    }

    /* Grow cache if needed and compute new terms */
    if ((size_t)n > len) {
        size_t required = (size_t)n;
        if (required > cap) {
            size_t new_cap = cap ? cap : 8;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute terms from len+1 up to n */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t term = (seq_t)(16 * cache[(size_t)(i - 2)]
                                - 65 * cache[(size_t)(i - 3)]
                                + 92 * cache[(size_t)(i - 4)]
                                - 48 * cache[(size_t)(i - 5)]
                                +  8 * cache[(size_t)(i - 6)]);
            cache[(size_t)(i - 1)] = term;
        }
        len = n;
    }

    return cache[(size_t)(n - 1)];
}
