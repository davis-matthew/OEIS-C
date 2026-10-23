/*
 * A107913  Numbers k that require three iterations of the sigma function to
 *          be >= 2*k.
 * https://oeis.org/A107913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer k (n≥1) such that
   sigma(sigma(sigma(k))) ≥ 2*k,
   sigma(sigma(k))        < 2*k,
   sigma(k)               < 2*k.
   (sigma is the sum‑of‑divisors function.  The sequence is A107913.) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma1.h"  /* seq_t sigma1(idx_t n) */

/* Return the n-th term of A107913.  Calls may occur in arbitrary order. */
seq_t a(idx_t n)
{
    static seq_t *terms    = NULL;     /* stored sequence values   */
    static idx_t  count    = 0;        /* number of terms stored   */
    static idx_t  capacity = 0;        /* current allocation size  */
    static idx_t  candidate = 1;       /* next integer to test     */

    /* Continue generating terms until we have at least n of them. */
    while (count < n)
    {
        /* Sigma of candidate */
        seq_t s1 = sigma1(candidate);
        seq_t two_k = (seq_t)candidate * 2;

        if (s1 < two_k)
        {
            /* Sigma of sigma(candidate) -- cast to idx_t because sigma1 expects idx_t */
            seq_t s2 = sigma1((idx_t)s1);
            if (s2 < two_k)
            {
                seq_t s3 = sigma1((idx_t)s2);
                if (s3 >= two_k)
                {
                    /* Store the candidate as a valid term. */
                    if (count == capacity)
                    {
                        capacity = (capacity == 0) ? 32 : capacity * 2;
                        seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                        if (!tmp) return 0;           /* allocation failed */
                        terms = tmp;
                    }
                    terms[count++] = (seq_t)candidate;
                }
            }
        }
        ++candidate;   /* move on to next integer */
    }

    return terms[n - 1];
}
