/*
 * A235001  Odious squares.
 * https://oeis.org/A235001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 154, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n-th perfect square k² whose binary representation
   has an odd number of 1‑bits.  The function scans k = 1,2,… and collects
   k² with odd popcount until the n‑th term is reached. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;            // next value of k to test

    if (n == 0) return 0;              // undefined for n=0, but safe

    while (cache_len < n) {
        seq_t sq = (seq_t)next_k * next_k;

        /* Determine parity of popcount(sq).  True if odd. */
        bool parity = false;
        seq_t tmp = sq;
        while (tmp) {
            parity ^= (bool)(tmp & 1);
            tmp >>= 1;
        }

        if (parity) {
            /* Grow the cache if needed. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_k;
    }

    return cache[n - 1];
}
