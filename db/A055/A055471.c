/*
 * A055471  Numbers that are divisible by the product of their nonzero digits.
 * https://oeis.org/A055471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that is divisible by the product
 *            of its non‑zero decimal digits
 * Range:     u8 n <= 49, u16 n <= 1073, u32 n <= 11442, u64 n <= 11442,
 *            u128 n <= 11442, big any n
 * Validated: standard, pass, 75 terms through n = 8220 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* public function: the nth term of A055471 */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t sz    = 0;   /* number of terms currently stored */
    static idx_t  last  = 1;   /* next integer to test */

    /* ensure we have a storage area */
    if (cache == NULL) {
        cap = 128;
        cache = mem_new(seq_t, cap);
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* extend cache while we haven't computed enough terms */
    while (sz < (size_t)n) {
        seq_t val = (seq_t)last;
        seq_t prod = 1;
        seq_t tmp  = val;

        /* compute product of non‑zero digits of val */
        while (tmp > (seq_t)0) {
            unsigned d = (unsigned)(tmp % (seq_t)10);
            if (d != 0) prod *= (seq_t)d;
            tmp /= (seq_t)10;
        }

        /* check divisibility */
        if (prod != (seq_t)0 && val % prod == (seq_t)0) {
            /* resize if needed */
            if (sz == cap) {
                size_t new_cap = cap * 2;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (new_cache == NULL) exit(EXIT_FAILURE);
                cache = new_cache;
                cap   = new_cap;
            }
            cache[sz++] = val;
        }

        ++last;
    }

    return cache[(size_t)n - 1];
}
