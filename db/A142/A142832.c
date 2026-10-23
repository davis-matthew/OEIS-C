/*
 * A142832  Primes congruent to 34 mod 61.
 * https://oeis.org/A142832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime ≡ 34 (mod 61)  */
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. We ignore n==0; behaviour below the offset is unspecified */
    if (n == 0)
        return 0;

    /* start from the smallest positive integer ≡ 34 (mod 61) */
    seq_t cand = 34;
    idx_t  count = 0;

    for (;; cand += (seq_t)61)          /* step through the arithmetic progression */
    {
        if (isPrimeGeneric(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
