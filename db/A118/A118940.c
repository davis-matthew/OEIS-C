/*
 * A118940  Primes p such that (p^2+7)/8 is prime.
 * https://oeis.org/A118940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 32, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118940: primes p such that (p^2+7)/8 is prime.
   The function returns the nth term (indexing starts at 1). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Below the offset behaviour is irrelevant; return 0 if requested. */
    if (n == 0) return 0;

    /* Static cache of terms found so far. */
    static seq_t *terms      = NULL;   /* array of sequence values */
    static size_t terms_capacity = 0;   /* allocated capacity */
    static size_t terms_count    = 0;   /* number of terms stored */

    /* Candidate generation state. */
    static uint64_t next_candidate = 3; /* start from 3 and step by 2 (odd) */

    /* Generate terms until we have at least n. */
    while (terms_count < n) {
        /* Find the next prime candidate > 2. */
        uint64_t p;
        do {
            p = next_candidate;
            next_candidate += 2;          /* only odd numbers */
        } while (!is_prime_small((seq_t)p)); /* fast small‑prime test */

        /* Compute (p^2 + 7) / 8 using 128‑bit intermediate to avoid overflow. */
        unsigned __int128 prod   = (unsigned __int128)p * p;
        unsigned __int128 q128   = (prod + 7) / 8;
        seq_t            q       = (seq_t)q128;      /* cast back to seq_t */

        /* Check that the derived number is prime. */
        if (is_prime_big(q)) {
            /* Ensure enough space in the cache. */
            if (terms_count == terms_capacity) {
                terms_capacity = terms_capacity ? terms_capacity * 2 : 16;
                terms = (seq_t *)realloc(terms,
                                          terms_capacity * sizeof(seq_t));
            }
            /* Store the new term. */
            terms[terms_count++] = (seq_t)p;
        }
    }

    return terms[n - 1];
}
