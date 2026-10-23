/*
 * A039194  Numbers whose base-11 representation has the same number of 4's
 *          and 10's.
 * https://oeis.org/A039194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth non‑negative integer whose base‑11
 *            representation contains the same number of 4's as 10's. The
 *            function computes this by incremental search and caching
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a base‑11 number has equal counts of digits 4 and 10 */
static bool good(seq_t x)
{
    idx_t c4 = 0, c10 = 0;
    seq_t four = 4, ten = 10, mod = 11;
    while (x > (seq_t)0) {
        seq_t d = x % mod;
        if (d == four)
            c4++;
        else if (d == ten)
            c10++;
        x /= mod;
    }
    return c4 == c10;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;     /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        if (cache_cap <= cache_size) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)      /* allocation failed – return 0 as safe fallback */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }
        if (good((seq_t)next_candidate))
            cache[cache_size++] = (seq_t)next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
