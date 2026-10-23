/*
 * A141114  Positive integers k where d(d(k)) is coprime to k, where d(k) is
 *          the number of divisors of k.
 * https://oeis.org/A141114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A141114: k such that d(d(k)) is coprime to k.
   The function generates the sequence by incremental search, caching
   all found terms. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of valid terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* capacity of the cache array   */
    static idx_t next_k = 1;       /* next integer to test          */

    /* Grow cache until it contains at least n terms */
    while ((idx_t)cache_len < n) {
        /* Expand storage if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute divisor counts at 64‑bit precision */
        uint64_t k_val = (uint64_t)next_k;
        uint64_t d1 = divisor_count_u64(k_val);
        uint64_t d2 = divisor_count_u64(d1);

        /* gcd(d2, k) == 1  =>  term found */
        if (gcd((seq_t)d2, (seq_t)next_k) == 1) {
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    /* Return the n‑th term (1‑based index) */
    return cache[n - 1];
}
