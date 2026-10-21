/*
 * A061179  Fourth column (m=3) of triangle A060920 (bisection of Fibonacci
 *          triangle, even part).
 * https://oeis.org/A061179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 58*a(n-2) + 144*a(n-3) - 195*a(n-4) + 144*a(n-5)
   - 58*a(n-6) + 12*a(n-7) - a(n-8)
   Initial values: a(0)=1, a(1)=14, a(2)=105, a(3)=594, a(4)=2860,
   a(5)=12402, a(6)=49963, a(7)=190570. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* first 8 terms known directly */
    static const seq_t init_vals[8] = {
        (seq_t)1,
        (seq_t)14,
        (seq_t)105,
        (seq_t)594,
        (seq_t)2860,
        (seq_t)12402,
        (seq_t)49963,
        (seq_t)190570
    };

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 8;
        cache_len = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
    }

    /* grow cache as needed to cover requested index */
    while ((idx_t)n >= (idx_t)cache_len) {
        /* compute next term using recurrence */
        seq_t pos = 12 * cache[cache_len - 1];
        pos += 144 * cache[cache_len - 3];
        pos += 144 * cache[cache_len - 5];
        pos += 12 * cache[cache_len - 7];

        seq_t neg = 58 * cache[cache_len - 2];
        neg += 195 * cache[cache_len - 4];
        neg += 58 * cache[cache_len - 6];
        neg += cache[cache_len - 8];

        seq_t next = pos - neg;   /* pos >= neg for all n >= 0 */

        /* resize array if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next;
    }

    return cache[(size_t)n];
}
