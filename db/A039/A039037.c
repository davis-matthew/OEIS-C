/*
 * A039037  Numbers whose base-7 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_ones_and_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039037
   a(n) = the n-th non‑negative integer whose base‑7 representation
   contains the same number of digits 1 and 4.  a(1)=0. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/equal_ones_and_fours.h"

/* Main sequence generator with lazy memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1, give 0 */

    /* static state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_tested = 0;
    static size_t matched = 0;

    if (cache_len == 0) {                    /* first call */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                       /* a(1) = 0 */
        cache_len = 1;
        matched = 1;
        last_tested = 0;
    }

    if ((size_t)n <= matched)                /* already computed */
        return cache[n - 1];

    seq_t candidate = last_tested + 1;
    while (matched < (size_t)n) {
        if (equal_ones_and_fours(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
            ++matched;
        }
        ++candidate;
    }

    last_tested = candidate - 1;
    return cache[n - 1];
}
