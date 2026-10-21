/*
 * A216590  Primes p with property that there exists a number d>0 such that
 *          numbers p-k*d, k=1...7, are seven primes.
 * https://oeis.org/A216590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via search over primes p such that there exists a
 *            positive integer d with 210 | d for which the seven numbers p −
 *            k*d (k = 1 … 7) are all primes
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for calls with n < 1 */

    /* cached results of the sequence */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* last prime used as a starting point for next search */
    static idx_t last_checked = 1;  /* prime 2 will be found next */

    /* generate terms up to the requested index */
    while (terms_len < (size_t)n)
    {
        /* get next prime */
        last_checked = next_prime(last_checked);

        /* d must be a multiple of 210; try all such d until
           p - 7*d < 2.  The maximum m is floor((p-2)/1470). */
        idx_t p = last_checked;
        idx_t m_max = (p > 2) ? (p - 2) / 1470 : 0;
        bool found = false;

        for (idx_t m = 1; m <= m_max && !found; ++m)
        {
            seq_t d = (seq_t)210 * (seq_t)m;
            bool ok = true;

            for (int k = 1; k <= 7; ++k)
            {
                seq_t val = (seq_t)p - (seq_t)k * d;
                if (!is_prime_big(val))
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                /* store the found prime */
                if (terms_len == terms_cap)
                {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms,
                                             terms_cap * sizeof(seq_t));
                    if (!terms) return 0;   /* allocation failure */
                }
                terms[terms_len++] = p;
                found = true;
            }
        }
        /* If no suitable d was found for this prime, simply go to the
           next prime.  The loop continues until the array contains
           enough terms. */
    }

    return terms[n - 1];
}
