/*
 * A115168  Even numbers n such that n-2 and n+2 have the same number of prime
 *          divisors with multiplicity.
 * https://oeis.org/A115168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Even numbers n such that n-2 and n+2 have the same number of
   prime divisors with multiplicity.  The function computes terms on demand
   and memoises them. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Local static state to memoise the sequence. */
    static seq_t *terms          = NULL;   /* array of found terms    */
    static size_t  terms_len     = 0;      /* number of terms stored  */
    static size_t  terms_cap     = 0;      /* current capacity of 'terms' */
    static seq_t   next_even     = 8;      /* next even number to test */

    if (n == 0)
        return 0;   /* undefined for 0, but return 0 to avoid UB */

    /* Ensure we have at least 'n' terms computed. */
    while (terms_len < (size_t)n)
    {
        /* Grow the storage if necessary. */
        if (terms_len + 1 > terms_cap)
        {
            terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!tmp)
            {
                /* In a real project, handle out‑of‑memory gracefully. */
                abort();
            }
            terms = tmp;
        }

        /* Check the property for the current candidate. */
        seq_t m_minus = next_even - 2;
        seq_t m_plus  = next_even + 2;
        if (count_prime_factors(m_minus) == count_prime_factors(m_plus))
        {
            /* Store the candidate as the next term. */
            terms[terms_len++] = next_even;
        }

        /* Move to the next even number. */
        next_even += 2;
    }

    return terms[(size_t)n - 1];
}
