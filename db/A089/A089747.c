/*
 * A089747  Numbers n such that n^2 - 2n + 5 is prime.
 * https://oeis.org/A089747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th non‑negative integer k such that k^2 − 2k + 5 is
 *            prime (offset 1, a(1)=0)
 * Range:     u8 n <= 44, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache of previously generated terms */
    static seq_t *terms      = NULL;
    static idx_t terms_len   = 0; /* number of terms currently cached   */
    static idx_t terms_cap   = 0; /* allocated capacity of the cache    */
    static idx_t next_cand   = 0; /* next even candidate to test        */

    /* Grow the cache to hold at least n terms if needed */
    if (n > terms_len) {
        /* Ensure sufficient capacity */
        if (terms_cap < n) {
            idx_t new_cap = terms_cap > 0 ? terms_cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            /* realloc casted to seq_t* to satisfy C++ */
            seq_t *tmp = (seq_t *)realloc(terms, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – return a null‑term */
                return (seq_t)0;
            }
            terms      = tmp;
            terms_cap  = new_cap;
        }

        /* Generate terms up to the requested index */
        while (terms_len < n) {
            /* Scan even candidates one by one */
            while (1) {
                idx_t cand = next_cand;
                next_cand += 2;  /* advance to the next even number */

                /* Compute cand^2 − 2*cand + 5 using 128‑bit arithmetic
                   to avoid overflow for reasonably large cand.       */
                idx_t val = (idx_t)(((__uint128_t)cand * cand) - 2 * cand + 5);

                if (isPrime(val)) {
                    /* Store the term and break to generate next one. */
                    terms[terms_len++] = (seq_t)cand;
                    break;
                }
            }
        }
    }

    return terms[n - 1];
}
