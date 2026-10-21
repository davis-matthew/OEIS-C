/*
 * A078174  Numbers with an integer arithmetic mean of distinct prime factors.
 * https://oeis.org/A078174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m ≥ 2 such that the arithmetic mean * of
 *            the distinct prime factors of m is an integer * * a(m) = m if
 *            sum of distinct primes dividing m ## omega(m) is * an integer
 *            (i.e., the sum is divisible by the number of * distinct primes);
 *            otherwise a(m) is the next such integer. * T...
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"

/* The cache of already-found terms (1‑based, a(1) stored at index 0) */
static seq_t *terms      = NULL;
static size_t terms_size   = 0;   /* number of terms currently stored   */
static size_t terms_cap    = 0;   /* allocated capacity                  */

/* The last integer that was examined during the search.  Starts
 * just before the first candidate (2) so that the first increment
 * moves to 2. */
static idx_t last_candidate = 1;

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms available before returning. */
    while (terms_size < n)
    {
        /* The next integer to test. */
        ++last_candidate;

        /* Factorize last_candidate.  We only need distinct primes
         * and their exponents.  The utility `factor_uint64` writes
         * the prime factors into the array `primes`, the exponents
         * into `exp`, and the number of distinct factors into `cnt`. */
        uint64_t primes[20];
        int       exp[20];
        int cnt = 0;                      /* number of distinct prime factors */

        factor_uint64((uint64_t)last_candidate, primes, exp, &cnt);

        /* Compute the remainder of the sum of distinct primes when
         * divided by the count.  If that remainder is zero, the mean
         * is an integer and the number qualifies. */
        int rem = 0;
        for (int i = 0; i < cnt; ++i)
            rem = (rem + (int)(primes[i] % cnt)) % cnt;

        if (rem == 0)
        {
            /* If the cache is full, grow it. */
            if (terms_size == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();               /* unrecoverable memory error */
                terms = tmp;
                terms_cap = new_cap;
            }

            /* Store the qualifying term.  The static cast (or
             * implicit conversion) relies on the fact that seq_t can
             * be constructed or assigned from a uint64_t value. */
            terms[terms_size++] = (seq_t)last_candidate;
        }
    }

    /* Return the requested term (sequence is 1‑based). */
    return terms[n - 1];
}
