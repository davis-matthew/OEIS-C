/*
 * A055471  Numbers that are divisible by the product of their nonzero digits.
 * https://oeis.org/A055471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that is divisible by the product
 *            of its non‑zero decimal digits
 * Range:     u8 n <= 49, u16 n <= 1073, u32 n <= 8220, u64 n <= 8220,
 *            u128 n <= 8220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

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
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
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
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
