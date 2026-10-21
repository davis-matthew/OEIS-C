/*
 * A201313  Primes of the form n^2 - 10.
 * https://oeis.org/A201313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 2202, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form n^2 - 10 (search) */

#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of primes found of the form k^2-10 */
    static seq_t *cache  = NULL;
    static size_t   cap  = 0;
    static size_t   len  = 0;
    static idx_t    next = 9;          /* first k giving a prime: 9^2-10 = 71 */

    /* Largest representable value of seq_t */
    const seq_t max_val = (seq_t)-1;

    while (len < n) {
        idx_t  k   = next;
        next += 2;                     /* skip even k: (even)^2-10 is even >2 */

        idx_t  cand64 = k * k - 10;     /* candidate in 64‑bit math */

        /* Skip candidates that do not fit into seq_t */
        if (sizeof(seq_t) < sizeof(idx_t) && cand64 > (idx_t)max_val)
            continue;

        seq_t cand = (seq_t)cand64;

        /* a candidate ≤ 1 cannot be prime */
        if (cand <= 1)
            continue;

        if (isPrimeGeneric(cand)) {
            /* Grow cache if needed */
            if (len == cap) {
                size_t new_cap = cap ? (cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
    }

    return cache[n - 1];   /* 1‑based indexing as per OEIS */
}
