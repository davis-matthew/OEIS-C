/*
 * A111230  Numbers k such that 5*k + 14 is prime.
 * https://oeis.org/A111230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that 5*k + 14 is prime
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  // offset is 1; return 0 for out‑of‑range

    static seq_t *cache = NULL;
    static idx_t size = 0;
    static idx_t cap  = 0;

    if (n > size) {
        /* grow cache to hold at least n elements */
        if (cap < n) {
            idx_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t start_k = 1;
        if (size > 0) {
            start_k = cache[size - 1] + 1;
        }

        for (seq_t k = start_k; size < n; ++k) {
            uint64_t pk = 5ULL * (uint64_t)k + 14ULL;
            if (is_prime(pk)) {
                cache[size++] = k;
            }
        }
    }
    return cache[n - 1];
}
