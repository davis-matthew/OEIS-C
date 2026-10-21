/*
 * A098807  Numbers k such that there exists a prime p for which p - k and p +
 *          k are also prime.
 * https://oeis.org/A098807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 6840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098807: Numbers k such that there exists a prime p with p−k
 * and p+k also prime.  For an even k not divisible by 3 this is
 * equivalent to k+3 and 2*k+3 being prime.  The function generates the
 * sequence lazily and caches the terms for subsequent calls. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms          */
    static size_t cache_cap = 0;  /* allocated capacity of cache      */
    static idx_t cache_len  = 0;  /* number of terms actually stored  */
    static uint64_t next_k   = 2; /* next even candidate to test      */

    /* trivial case: undefined offset – return 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* generate terms until we have at least n */
    while (cache_len < n) {
        uint64_t k = next_k;
        next_k += 2;                 /* advance to next even number    */

        bool term = false;
        if (k % 6 == 0) {            /* all multiples of 6 are terms    */
            term = true;
        } else if (k % 3 != 0) {     /* otherwise, test the prime cond. */
            /* Use isPrimeGeneric on 64‑bit values; it supports all
             * seq_t sizes that are representable in uint64_t.  For
             * larger seq_t builds the helper is compiled with the
             * appropriate type.                */
            seq_t a1 = (seq_t)(k + 3);
            seq_t b1 = (seq_t)(2 * k + 3);
            term = isPrimeGeneric(a1) && isPrimeGeneric(b1);
        }

        if (term) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap) ? (cache_cap << 1) : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* allocation failed – abort with zero value */
                    return (seq_t)0;
                }
                if (cache) {
                    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
