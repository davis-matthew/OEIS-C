/*
 * A045170  Numbers whose base-5 representation contains exactly three 0's and
 *          one 1.
 * https://oeis.org/A045170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑5 representation *
 *            contains exactly three 0's and one 1. The function generates the
 *            * sequence lazily and caches results to allow arbitrary calling
 *            order
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether a base‑5 number has exactly three 0's and one 1.  */
static bool good(uint64_t x)
{
    unsigned zeros = 0, ones = 0;
    while (x) {
        unsigned d = x % 5;
        if (d == 0) ++zeros;
        else if (d == 1) ++ones;
        x /= 5;
    }
    return zeros == 3 && ones == 1;
}

/* Return a(n) for the OEIS sequence A045170.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_len = 0;     /* number of cached terms   */
    static size_t cache_cap = 0;     /* current capacity         */
    static uint64_t cur = 1;         /* next candidate to test   */

    /* Grow the cache until we have at least n terms.  */
    while (cache_len < (size_t)n) {
        if (good(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;   /* store the term   */
        }
        ++cur;
    }

    return cache[(size_t)n - 1];
}
