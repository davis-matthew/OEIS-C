/*
 * A037389  Every base 4 digit of k is a base 7 digit of k.
 * https://oeis.org/A037389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 9935, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037389:
   numbers whose set of base‑4 digits is a subset of their set of base‑7 digits.
   The function builds the sequence lazily, caching previously found terms. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache      = NULL;   /* array of values                            */
    static size_t cache_size = 0;      /* number of values stored                    */
    static size_t cache_cap  = 0;      /* allocated capacity                         */

    /* Ensure the cache can hold at least n terms */
    if (cache_size < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;             /* double until large enough                 */

        /* Reallocate (safe cast for C++ as well) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* simple error handling */
            free(cache);
            abort();
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache contains n elements */
    seq_t k = cache_size ? cache[cache_size - 1] + 1 : 1;    /* next candidate  */
    while (cache_size < (size_t)n) {
        seq_t num = k;

        /* mask of base‑4 digits */
        uint8_t mask4 = 0;
        seq_t t = num;
        while (t != 0) {
            int d = (int)(t % 4);
            mask4 |= 1U << d;
            t /= 4;
        }

        /* mask of base‑7 digits */
        uint8_t mask7 = 0;
        t = num;
        while (t != 0) {
            int d = (int)(t % 7);
            mask7 |= 1U << d;
            t /= 7;
        }

        /* If every digit present in base‑4 appears in base‑7, store it */
        if ((mask4 & ~mask7) == 0)
            cache[cache_size++] = num;

        k = num + 1;   /* proceed to next candidate */
    }

    return cache[n - 1];
}
