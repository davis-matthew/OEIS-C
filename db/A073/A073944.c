/*
 * A073944  a(n) is the smallest m such that n-th prime divides m! + 1.
 * https://oeis.org/A073944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = minimal m such that the n‑th prime p_n divides m!+1.
   We find it by iterating m! modulo p_n until it is congruent to p_n‑1. */
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, undefined for 0 */
    size_t idx = (size_t)(n - 1);          /* zero based index */

    if (idx >= cache_size) {
        /* enlarge cache to hold values up to idx */
        size_t new_size = idx + 1;
        if (new_size > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < new_size) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = new_cache;
            cache_cap = new_cap;
        }
        /* compute missing entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            uint32_t p = get_prime((idx_t)(i + 1));   /* nth prime, 1‑based */
            uint32_t r = 1 % p;
            seq_t min_m = 0;
            for (uint32_t m = 1; m < p; ++m) {
                r = (uint32_t)(((uint64_t)r * m) % p);
                if (r == p - 1) {
                    min_m = (seq_t)m;
                    break;
                }
            }
            seq_cache[i] = min_m;
        }
        cache_size = new_size;
    }
    return seq_cache[idx];
}
