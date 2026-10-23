/*
 * A136152  Composites one larger than a prime and with exactly three distinct
 *          prime factors.
 * https://oeis.org/A136152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = composite numbers n such that n-1 is prime and * n has
 *            exactly three distinct prime factors. OEIS offset 1
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t last_checked_prime = 1;   /* primes are >1 */

    /* we assume n >= 1; behaviour for n < 1 is undefined */
    if (n == 0)
        return 0;

    /* generate sequence values up to the requested index */
    while (seq_len < (size_t)n)
    {
        /* find next prime */
        last_checked_prime = next_prime(last_checked_prime);

        /* candidate is prime+1 */
        uint64_t cand = last_checked_prime + 1;

        /* check that cand has exactly three distinct prime factors */
        if (count_distinct_primes(cand) == 3)
        {
            /* grow cache if necessary */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;
        }
    }

    /* return the cached value */
    return seq[n - 1];
}
