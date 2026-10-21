/*
 * A089767  Squares which when concatenated with a 1 gives prime.
 * https://oeis.org/A089767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = m^2, where 10*m^2 + 1 is prime a(1)=1, a(2)=4, a(3)=25, …
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 7983, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Cache for computed terms */
    static seq_t *cache = NULL;    /* array of terms (a(1), a(2), …) */
    static idx_t  cached = 0;      /* number of terms stored */
    static size_t cap    = 0;      /* allocated capacity */
    static idx_t  next_m = 1;      /* next m to test (m = 1,2,3,…) */

    /* Generate terms until we have at least n of them */
    while (cached < n)
    {
        /* Compute 10*m^2 + 1 using 128‑bit to avoid overflow */
        unsigned __int128 expr = (unsigned __int128)next_m * next_m * 10 + 1;

        /* If the value no longer fits in uint64_t, stop.
           For the bounds used in the tests (< ~10000 terms) this
           situation never occurs. */
        if (expr > UINT64_MAX)
            break;

        uint64_t candidate = (uint64_t)expr;

        if (is_prime(candidate))
        {
            /* Store the square; conversion to seq_t may truncate on small types. */
            seq_t term = (seq_t)next_m * (seq_t)next_m;

            if (cached == cap)        /* extend cache storage */
            {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap   = new_cap;
            }
            cache[cached++] = term;
        }
        ++next_m;
    }

    return cache[n - 1];
}
