/*
 * A134427  Numbers k such that k^2 + 1 is a composite squarefree number.
 * https://oeis.org/A134427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 399, u32 n <= 399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth positive integer k such that k^2+1 is composite and squarefree. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* static cache of known terms */
    static seq_t *cache = NULL;   /* array of k values */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t last_checked_k = 0; /* last k that was examined */

    if (n == 0) return 0; /* undefined for offset 0, but harmless */

    /* enlarge cache if needed during growth */
    while (cache_len < n) {
        idx_t k = ++last_checked_k;
        seq_t val = (seq_t)k * (seq_t)k + 1;
        if (!is_prime_big(val) && is_squarefree(val)) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
