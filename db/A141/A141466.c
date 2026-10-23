/*
 * A141466  Nonprime transformed products of prime factors of the composites,
 *          the largest and smallest prime decremented by 1.
 * https://oeis.org/A141466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   _smallest_prime_factor, gpf, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A141466: iterate over composites, apply the
   transformation to the prime factors, and check for non‑prime
   products.  No memoisation is used; the function scans from the
   beginning for each call, which is fast enough for n ≤ 10 000.  */

/* Required data types are supplied by the build system.  */
#include <stdint.h>
#include <stdbool.h>

/* Helper functions provided by the framework.  */
#include "../utils/is_prime_small.h"    /* bool is_prime_small(seq_t)  */
#include "../utils/_smallest_prime_factor.h"  /* uint64_t _smallest_prime_factor(uint64_t) */
#include "../utils/gpf.h"              /* uint64_t gpf(uint64_t)     */

seq_t a(idx_t n)
{
    /* Count of non‑prime transformed products seen so far. */
    idx_t count = 0;

    /* Enumerate successive positive integers, starting from the first
       composite number 4. */
    for (uint64_t k = 4; ; ++k)
    {
        /* Skip primes – only composites contribute. */
        if (is_prime_small((seq_t)k))
            continue;

        /* Prime factorisation information.  */
        uint64_t pmin = _smallest_prime_factor(k);
        uint64_t pmax = gpf(k);

        /* Compute the transformed product:
               product = k/(pmin*pmax) * (pmax-1) * (pmin-1)   */
        seq_t div   = (seq_t)k / ((seq_t)pmin * (seq_t)pmax);
        seq_t prod  = div * (seq_t)(pmax - 1) * (seq_t)(pmin - 1);

        /* If the product is not prime, we count it. */
        if (!is_prime_small(prod))
        {
            count++;
            if (count == n)
                return prod;
        }
    }
}
