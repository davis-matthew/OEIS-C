/*
 * A232268  Numbers n such that reversal (n^3) plus 1 is prime.
 * https://oeis.org/A232268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the nth positive integer n such that reverse(n³)+1 is
 *            prime
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, reverse_decimal, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/reverse_decimal.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* Compute the n‑th term of sequence A232268.  The function is
   memoised so that calls for arbitrary n in any order are efficient. */
seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;   /* cache of found terms (1‑based logic) */
    static size_t terms_len  = 0;      /* number of terms currently stored   */
    static size_t terms_cap  = 0;      /* allocated capacity of terms array   */
    static idx_t next_candidate = 1;   /* next integer to test              */

    /* Build enough terms to cover the requested index. */
    while (terms_len < (size_t)n) {
        idx_t m = next_candidate++;          /* candidate integer */

        /* Compute m³ in an idx_t to avoid overflow, then cast to seq_t. */
        idx_t cube_idx = m;
        cube_idx *= m;
        cube_idx *= m;
        seq_t cube = cube_idx;

        /* Reverse the cube and add 1. */
        seq_t rev = reverse_decimal(cube);
        seq_t candidate = rev + (seq_t)1;

        /* Test primality of candidate. */
        if (isPrimeGeneric(candidate)) {
            /* Ensure capacity for another term. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 1;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) abort();           /* allocation failure */
                terms      = tmp;
                terms_cap  = new_cap;
            }
            terms[terms_len++] = m;           /* store the term */
        }
    }

    return terms[n - 1];
}
