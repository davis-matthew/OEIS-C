/*
 * A062848  When expressed in base 2 and then interpreted in base 7, is a
 *          multiple of the original number.
 * https://oeis.org/A062848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers x such that the binary representation of x,
   interpreted as a base‑7 number, is a multiple of x.  a(n) is the
   n‑th such number. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next number to test for the property */
    static idx_t next_candidate = 1;   /* start at 1, 0 is handled separately */

    /* initialize cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */
        cache[0]   = (seq_t)0;  /* a(1) = 0 */
        cache_len  = 1;
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        idx_t k = next_candidate++;    /* candidate number to test */

        /* determine most‑significant‑bit position of k */
        int msb = 0;
        idx_t t  = k;
        while ((t >>= 1) != 0) ++msb;

        /* test whether binary k, read in base 7, is a multiple of k */
        idx_t mod = 0;
        for (int i = msb; i >= 0; --i) {
            idx_t bit = (k >> i) & 1ULL;
            mod = (unsigned __int128)mod * 7 + bit;
            mod %= k;
        }

        if (mod == 0) {          /* k satisfies the definition */
            /* grow cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
