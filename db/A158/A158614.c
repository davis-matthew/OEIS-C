/*
 * A158614  Numbers n such that 30*n + 11 is prime.
 * https://oeis.org/A158614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1119, u32 n <= 67, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158614: numbers n such that 30*n + 11 is prime
   (search).  The function returns the n‑th such integer, where
   a(1) = 0, a(2) = 1, a(3) = 2, …  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity      */
    static idx_t  next_m     = 0;   /* next candidate to test */

    /* offset is 1 – behaviour for n==0 is irrelevant, but we guard it */
    if (n == 0)
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* candidate m */
        seq_t p = (seq_t)30 * (seq_t)next_m + (seq_t)11;   /* 30*m + 11 */

        if (is_prime_big(p)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* assume allocation succeeds */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_m;   /* store the valid n */
        }
        ++next_m;   /* advance to next candidate */
    }

    return cache[n - 1];   /* a(1) is at index 0 of cache */
}
