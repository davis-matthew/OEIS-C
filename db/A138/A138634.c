/*
 * A138634  Nonnegative integers k such that 17*k-9 is prime.
 * https://oeis.org/A138634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th non‑negative integer k with 17*k-9 prime
   (OEIS A138634).  a(1)=4, a(2)=8, ... */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;   /* candidate k to test next */

    if (n == 0) return 0;      /* offset starts at 1 */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find the next k such that 17*k-9 is prime */
        while (1) {
            seq_t val = (seq_t)17 * (seq_t)next_k - (seq_t)9;
            if (is_prime((idx_t)val)) {
                /* store k in the cache */
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
                cache[cache_len++] = (seq_t)next_k;
                ++next_k;  /* move to the next candidate for future calls */
                break;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
