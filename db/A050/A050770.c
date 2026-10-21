/*
 * A050770  Iterated procedure 'composite k added to sum of its prime factors
 *          reaches a prime' yields 3 skipped primes.
 * https://oeis.org/A050770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big, sum_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative process: for each composite k, compute p = k + sum of its prime factors (with multiplicity). If p is prime and there are exactly three primes strictly between k and p, then k is a term of A050770. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/sum_prime_factors.h"

seq_t a(idx_t n)
{
    /* Static array holding computed terms. */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Bookkeeping of next composite to test. */
    static idx_t next_k = 4;

    while (len <= n)
    {
        idx_t k = next_k;

        /* Scan composites until we find a qualifying term. */
        for (;; ++k)
        {
            if (is_prime_big((seq_t)k))
                continue;   /* skip primes */

            uint64_t s = sum_prime_factors(k);
            idx_t p = k + s;

            if (!is_prime_big((seq_t)p))
                continue;   /* end must be prime */

            /* Count primes strictly between k and p, stop if >3. */
            unsigned cnt = 0;
            for (idx_t x = k + 1; x < p; ++x)
            {
                if (is_prime_big((seq_t)x))
                {
                    if (++cnt > 3)
                        break;
                }
            }

            if (cnt == 3)
            {
                /* Store the term. */
                if (len == cap)
                {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    cap = new_cap;
                }
                seq[len++] = (seq_t)k;

                /* Next search starts after this term. */
                next_k = k + 1;
                break;   /* inner loop break, outer loop continues if needed */
            }
        }
    }

    return seq[n];
}
