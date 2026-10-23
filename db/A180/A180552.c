/*
 * A180552  Primes that become a different prime under the mapping 5 <=> 7.
 * https://oeis.org/A180552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 411, u32 n <= 291, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, map_5_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A180552
 *  Primes p such that the digit mapping 5 ↔ 7 applied to the decimal
 *  expansion of p yields a different prime.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/map_5_7.h"

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 */

    struct cache_s {
        seq_t *data;
        size_t len;
        size_t cap;
    };

    static struct cache_s cache = {0, 0, 0};
    static seq_t last_checked = 2; /* start from the first prime candidate */

    /* Allocate cache on first use. */
    if (cache.data == NULL) {
        cache.cap = 128;
        cache.data = (seq_t *)malloc(cache.cap * sizeof(seq_t));
        if (!cache.data)
            abort();
    }

    /* Extend cache until we have at least n terms. */
    while (cache.len < n) {
        /* Test current candidate. */
        if (is_prime_big(last_checked)) {
            seq_t mapped = map_5_7(last_checked);
            if (mapped != last_checked && is_prime_big(mapped)) {
                /* Store the candidate. */
                if (cache.len == cache.cap) {
                    size_t new_cap = cache.cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache.data, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();
                    cache.data = tmp;
                    cache.cap = new_cap;
                }
                cache.data[cache.len++] = last_checked;
            }
        }
        /* Move to next integer. */
        last_checked += 1;
    }

    return cache.data[n - 1];
}
