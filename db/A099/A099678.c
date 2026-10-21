/*
 * A099678  Numbers n such that 10n01 is a prime.
 * https://oeis.org/A099678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal concatenation 10 n 01 is prime.
   Implementation: incremental search storing found terms for subsequent
   queries.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static storage for generated sequence values */
    static seq_t *terms = NULL;      /* dynamically expanded array of n */
    static size_t terms_len = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* allocated capacity */
    static idx_t next_candidate = 1; /* next n to test */

    /* extend storage until we have at least n terms */
    while (terms_len < (size_t)n)
    {
        /* determine number of decimal digits of current candidate */
        int d = (int)digits(next_candidate);

        /* build the number 10n01 */
        seq_t candidateN =
            (seq_t)10 * pow10((idx_t)(d + 2)) + (seq_t)next_candidate * 100 + (seq_t)1;

        /* test primality of the constructed number */
        if (is_prime_big(candidateN))
        {
            /* grow array if necessary */
            if (terms_len == terms_cap)
            {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = next_candidate;
        }

        /* advance to next candidate n */
        next_candidate++;
    }

    /* return the nth term (1‑based index) */
    return terms[(size_t)n - 1];
}
