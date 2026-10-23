/*
 * A037973  Numbers whose maximal base 3 run length is 2.
 * https://oeis.org/A037973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_good_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037973 – Numbers whose maximal base‑3 run length is exactly 2.
 * This function finds the nth such number (n ≥ 1) by brute‑force searching in
 * base‑3, caching all found values for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_good_base3.h"

/* External linkage as required by the challenge */
seq_t a(idx_t n)
{
    /* Handle trivial case outside the official offset – behavior is undefined
     * but we return 0 to avoid crashes. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cache_cap = 0;      /* capacity of cache array */
    static seq_t next_candidate = 1; /* next integer to test */

    /* Return cached value if already known */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Search for the next good number */
        while (true) {
            seq_t val = next_candidate++;
            if (is_good_base3(val)) {
                /* Grow cache array if needed */
                if (cache_len == cache_cap) {
                    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(
                        cache, (size_t)new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();                /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                break;                         /* found next value */
            }
        }
    }
    return cache[(size_t)(n - 1)];
}
