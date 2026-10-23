/*
 * A167840  Six-times-isolated primes: primes p such that none of p+-2, p+-4,
 *          p+-6, p+-8, p+-10 nor p+-12 are prime.
 * https://oeis.org/A167840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167840 (Six-times-isolated primes)
   a(n) = the nth prime p such that none of p±2, p±4, p±6, p±8, p±10
   nor p±12 are prime.  Offset 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Find the n-th six-times-isolated prime.  The results are
   memoized in a static cache so that non‑monotonic calls are
   efficient. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 2;   /* next candidate to examine */

    if (n == 0) {
        /* Undefined for n < 1; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next prime starting from 'cur' */
        seq_t p = cur;
        while (!isPrimeGeneric(p)) {
            p += (seq_t)1;
        }
        cur = p + (seq_t)1;   /* set start for the next search */

        /* Check isolation: no p±d is prime for d = 2,4,6,8,10,12 */
        bool isolated = true;
        for (int d = 2; d <= 12; d += 2) {
            seq_t offset = (seq_t)d;
            seq_t ap = p + offset;
            if (isPrimeGeneric(ap)) {
                isolated = false;
                break;
            }
            if (p > offset) {
                seq_t am = p - offset;
                if (isPrimeGeneric(am)) {
                    isolated = false;
                    break;
                }
            }
        }
        if (!isolated) {
            continue;   /* not a member of the sequence */
        }

        /* Store the isolated prime */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – abort */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = p;
    }

    /* Return the requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
