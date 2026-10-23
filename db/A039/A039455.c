/*
 * A039455  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 9's.
 * https://oeis.org/A039455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑11 representation has an equal,
 * non‑zero number of 1's and 9's. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a base‑11 representation of x has the same
 * non‑zero counts of digits 1 and 9. */
static bool is_good(seq_t x)
{
    int cnt1 = 0;
    int cnt9 = 0;
    while (x > 0) {
        int d = (int)(x % 11);
        if (d == 1) ++cnt1;
        else if (d == 9) ++cnt9;
        x /= 11;
    }
    return (cnt1 == cnt9) && (cnt1 > 0);
}

/* External linkage as required.  The function generates terms
 * on demand and caches them for repeated use. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms      */
    static idx_t cache_len = 0;   /* number of cached terms       */
    static idx_t cache_cap = 0;   /* current capacity of cache    */
    static seq_t next_candidate = 0;   /* next integer to test       */

    /* Ensure at least n terms are cached.  Generation is linear
     * in the number of candidates examined. */
    while (cache_len < n) {
        ++next_candidate;
        if (is_good(next_candidate)) {
            if (cache_len + 1 > cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
                while (new_cap <= cache_len + 1)
                    new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
    }

    return cache[n - 1];
}
