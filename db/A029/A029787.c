/*
 * A029787  Numbers k such that k^2 and k^3 do not have any common digits.
 * https://oeis.org/A029787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k² and k³ have no common decimal digits */
/* Implementation: brute‑force search with caching. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_k   = 2;          /* start searching from 2 */

    if (n <= cache_len)          /* already computed terms are cached */
        return cache[n-1];

    if (cache_cap == 0) {        /* initial allocation */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        seq_t k  = (seq_t)next_k;
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;

        /* digit mask of k² */
        unsigned mask2 = 0;
        seq_t tmp = k2;
        while (tmp) {
            mask2 |= 1U << (unsigned)(tmp % 10);
            tmp    = tmp / 10;
        }

        /* digit mask of k³ */
        unsigned mask3 = 0;
        tmp = k3;
        while (tmp) {
            mask3 |= 1U << (unsigned)(tmp % 10);
            tmp    = tmp / 10;
        }

        if ((mask2 & mask3) == 0) {            /* no common digit */
            if (cache_len == cache_cap) {      /* grow cache if needed */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }

        ++next_k;
    }

    return cache[n-1];
}
