/*
 * A089193  Odd numbers n such that 2*n-7 is a prime of the form 4*k+3.
 * https://oeis.org/A089193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odd n such that 2*n-7 is a prime of the form 4*k+3
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache for sequence values (1‑based) */
    static seq_t *cache        = NULL;   /* array of found terms      */
    static size_t cache_size   = 0;      /* number of terms stored    */
    static size_t cache_cap    = 0;      /* array capacity            */
    static idx_t next_candidate = 5;     /* next odd number to test   */

    if (n < 1) {
        return 0;   /* behaviour below the offset is irrelevant */
    }

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 128;                    /* arbitrary starting size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        idx_t p64 = next_candidate * 2ULL - 7ULL;   /* 2*n-7 */

        if ((p64 & 3ULL) == 3ULL &&             /* p ≡ 3 (mod 4)   */
            isPrimeGeneric((seq_t)p64)) {       /* p is prime       */
            /* grow the cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);              /* bailout on OOM */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 2ULL;                 /* next odd number   */
    }

    return cache[n - 1];   /* 1‑based indexing */
}
