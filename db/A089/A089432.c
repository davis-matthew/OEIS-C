/*
 * A089432  Numbers n such that n concatenated with floor(n/2) is prime.
 * https://oeis.org/A089432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the decimal concatenation of
 *            k and floor(k/2) is prime. Implementation: linear search with
 *            memoisation
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digits, isPrimeGeneric, pow10_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digits.h"          /* int digits(idx_t x);          */
#include "../utils/isPrimeGeneric.h"  /* bool isPrimeGeneric(seq_t);   */
#include "../utils/pow10_u128.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Cache of previously discovered terms. */
    static seq_t *cache     = NULL;
    static size_t len       = 0;        /* number of terms stored.   */
    static size_t cap       = 0;        /* allocated capacity.       */
    static idx_t  next      = 1;        /* next candidate to test.   */

    /* Grow storage if necessary. */
    if (len == cap) {
        cap = (cap == 0) ? 64 : cap << 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        /* Find next odd candidate. */
        while (true) {
            idx_t cand = next++;          /* candidate n. */
            if ((cand & 1) == 0)         /* even n cannot work. */
                continue;

            /* floor(cand/2) */
            idx_t half = cand >> 1;

            /* Number of decimal digits of half. */
            int d = digits(half);

            /* Concatenate: cand * 10^d + half.  */
            unsigned __int128 concat128 =
                (unsigned __int128)cand * pow10_u128(d) + (unsigned __int128)half;

            /* If the concatenated value does not fit in seq_t, skip it.
               This only happens for very large candidates; all terms that
               fit in seq_t are processed correctly. */
            if (concat128 > (unsigned __int128)~(seq_t)0)
                continue;

            seq_t concat = (seq_t)concat128;

            if (isPrimeGeneric(concat)) {
                cache[len++] = (seq_t)cand;   /* store the term. */
                break;                       /* move to next required term. */
            }
        }
    }

    /* Return the requested term (1‑based indexing). */
    return cache[n - 1];
}
