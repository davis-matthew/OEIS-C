/*
 * A084089  Numbers k such that k == 1 (mod 3) and the exponent of the highest
 *          power of 2 dividing k is even.
 * https://oeis.org/A084089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084089: Numbers k such that k ≡ 1 (mod 3) and the exponent of the
   highest power of 2 dividing k is even.  This implementation
   generates terms by brute force, keeping a cache of found values. */

#include <stdlib.h>
#include <stdint.h>

/* a(1) = 1, a(2) = 4, a(3) = 7, … */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1 (a(1) is the first term). */
    if (n == 0) return 0;   /* value for index 0 is irrelevant */

    /* Cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static uint64_t testk = 1;     /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k = testk++;
        /* Condition k ≡ 1 (mod 3) */
        if ((k % 3) != 1) continue;

        /* Compute v2(k) – exponent of the highest power of 2 dividing k */
        unsigned v = 0;
        uint64_t t = k;
        while ((t & 1) == 0) {
            t >>= 1;
            ++v;
        }

        /* v must be even (including v = 0 when k is odd) */
        if ((v & 1) != 0) continue;

        /* Ensure storage capacity */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            /* In a rare OOM condition just leave the pointer unchanged;
               the sequence is not required to handle that case. */
            if (tmp) cache = tmp;
        }

        cache[cache_len++] = (seq_t)k;   /* store the found term */
    }

    /* Return the requested term (1‑based indexing). */
    return cache[(size_t)(n - 1)];
}
