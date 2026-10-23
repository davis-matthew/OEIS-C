/*
 * A166563  Numbers n such that 12*n+5 is not prime.
 * https://oeis.org/A166563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer m such that 12*m + 5 is composite
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static table of found terms (zero‑based indexing) */
    static seq_t *cache    = NULL;   /* array of m values */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t last_checked = 0;   /* largest m examined so far */

    if (n == 0) /* offset is 1, but guard against zero for safety */
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        last_checked += 1; /* next candidate m to test */

        /* Compute 12*m + 5 as a seq_t value before primality test */
        seq_t test = (seq_t)12 * last_checked + 5;

        if (!isPrimeGeneric(test)) { /* composite -> record this m */
            /* Ensure enough space for one more element */
            grow_seq(&cache, &cache_cap, cache_len + 1);

            /* Store the term as a seq_t (conversion from idx_t is safe) */
            cache[cache_len] = (seq_t)last_checked;
            cache_len += 1;
        }
    }

    return cache[n - 1]; /* return the nth term */
}
