/*
 * A039271  Numbers whose base-12 representation has the same number of 8's
 *          and 11's.
 * https://oeis.org/A039271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same number of 8's and 11's
 * a(n) = the n‑th smallest nonnegative integer with equal count of digits 8 and 11
 * in base 12.  Computation by brute‑force search of successive integers. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* stored terms so far */
    static size_t cache_len  = 0;      /* number of terms in cache   */
    static size_t cache_cap  = 0;      /* allocated capacity         */
    static seq_t  next       = 0;      /* next candidate to test    */
    static bool   initialized = false;

    if (!initialized) {
        /* start with a small capacity */
        cache_cap = 64;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next      = 0;
        initialized = true;
    }

    /* Ensure we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        seq_t x = next;
        unsigned int c8  = 0;
        unsigned int c11 = 0;
        seq_t tmp = x;

        /* Count digits 8 and 11 in base‑12 representation */
        if (tmp != 0) {
            while (tmp) {
                seq_t d = tmp % 12;
                if (d == 8)  c8++;
                else if (d == 11) c11++;
                tmp /= 12;
            }
        }
        /* zero has zero 8's and zero 11's, so it qualifies */

        if (c8 == c11) {
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
        next++;
    }

    return cache[n - 1];
}
