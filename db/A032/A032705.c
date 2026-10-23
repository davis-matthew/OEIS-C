/*
 * A032705  Numbers k such that k prefixed by '5' and followed by '1' is
 *          prime.
 * https://oeis.org/A032705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032705: numbers k such that the decimal concatenation 5 k 1
   forms a prime. This implementation searches k in increasing order
   and memoises the results so that calls with arbitrary n are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t) */
#include "../utils/pow10.h"            /* seq_t pow10(idx_t) */

seq_t a(idx_t n)
{
    /* memoisation cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of cache   */
    static size_t cache_len = 0;   /* number of elements stored     */

    /* candidate generation state */
    static idx_t cand_k = 1;          /* next k to test                */
    static idx_t next_boundary = 10; /* 10^(digits)                   */
    static idx_t digits = 1;          /* current number of digits      */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* when cand_k reaches the next power of ten, increase digit count */
        if (cand_k == next_boundary) {
            ++digits;
            /* next_boundary may overflow for huge k; this is harmless
               for n up to ≈10⁴ (k stays below 10⁶). */
            next_boundary *= 10;
        }

        /* candidate = 5 * 10^(digits+1) + k * 10 + 1 */
        seq_t base      = pow10((idx_t)(digits + 1));
        seq_t candidate = (seq_t)5 * base + (seq_t)cand_k * (seq_t)10 + (seq_t)1;

        if (isPrimeGeneric(candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort(); /* out of memory */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)cand_k;
        }

        ++cand_k;
    }

    return cache[n - 1];
}
