/*
 * A153085  Numbers k such that 4*k + 5 is not prime.
 * https://oeis.org/A153085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 4*k + 5 is composite. The function finds the
 *            n‑th integer k with this property by iterating k from 0 upwards
 *            and testing 4*k+5 for primality. Results are memoised so that
 *            calls with arbitrary indices are fast
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;           /* next k to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = seq_alloc(cache_cap);
    }

    /* generate terms until we have at least n */
    while (cache_len < n) {
        seq_t val = (seq_t)4 * (seq_t)next_k + 5;
        if (!is_prime_big(val)) {            /* composite -> store k */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = seq_realloc(cache, cache_cap / 2, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
