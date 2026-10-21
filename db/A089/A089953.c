/*
 * A089953  Numbers n such that 3*n+7 is prime.
 * https://oeis.org/A089953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th non‑negative integer m such that 3*m+7 is prime
 *            (with a(1)=0)
 * Range:     u8 n <= 66, u16 n <= 8847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th element of OEIS A089953.  a(1) is the first term. */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of already found terms. */
    static seq_t *cache     = NULL;
    static size_t cap       = 0;
    static size_t len       = 0;

    /* The next candidate integer m to test; keep state between calls. */
    static idx_t next = 0;   /* starts at 0 */

    /* Grow the cache until it contains at least n terms. */
    while (len < n) {
        idx_t m = next;                /* current candidate m */
        idx_t p = 3 * m + 7;           /* value to test for primality */

        if (is_prime(p)) {             /* m is a valid term */
            /* Expand the cache if necessary. */
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();    /* memory failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)m;    /* store the m value */
        }
        ++next;                         /* move to the next candidate */
    }

    return cache[n - 1];                /* a(1) is cache[0] */
}
