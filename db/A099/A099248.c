/*
 * A099248  Numbers such that length in binary representation and number of
 *          ones have a common divisor greater than 1.
 * https://oeis.org/A099248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for numbers whose binary length and popcount have gcd > 1. */
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_scanned = 0;

    if (n == 0) return (seq_t)0;  /* outside of defined offset */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t x = last_scanned + 1;

        /* compute bit length of x */
        unsigned bitlen = 0;
        seq_t tmp = x;
        while (tmp) {
            bitlen++;
            tmp >>= 1;
        }

        /* compute popcount of x */
        unsigned ones = 0;
        tmp = x;
        while (tmp) {
            ones += (unsigned)(tmp & (seq_t)1);
            tmp >>= 1;
        }

        if (gcd((seq_t)bitlen, (seq_t)ones) > (seq_t)1) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }

        last_scanned = x;
    }

    return cache[n-1];
}
