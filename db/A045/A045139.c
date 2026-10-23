/*
 * A045139  Numbers whose base-4 representation contains exactly one 2 and one
 *          3.
 * https://oeis.org/A045139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly one digit 2
 *            and one digit 3
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_one_2_and_one_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_2_and_one_3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1; caller should use n >= 1 */

    /* persistent static cache */
    static idx_t cache_len = 0;          /* number of terms cached */
    static seq_t *cache = NULL;          /* array of cached terms */
    static size_t cache_cap = 0;         /* capacity of cache array */
    static seq_t next_candidate = 0;     /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
    }

    /* generate terms until we have at least n terms cached */
    while (cache_len < n) {
        /* find the next valid number */
        while (!has_one_2_and_one_3(next_candidate)) {
            ++next_candidate;
        }

        /* expand cache if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;    /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the valid number */
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
