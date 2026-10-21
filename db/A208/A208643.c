/*
 * A208643  Least positive integer m such that those k*(k-1) mod m with
 *          k=1,...,n are pairwise distinct.
 * https://oeis.org/A208643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest m > 2n-2 such that m is prime or a power of two */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 2n-2 is even, so the minimal candidate is 2n-1.
       For n = 1 this is 1, which is 2⁰. */
    idx_t m = 2*n - 1;
    for (;; ++m)
    {
        /* Check for power of two (including 1). */
        if ((m & (m-1)) == 0)
            return (seq_t)m;
        /* Check for primality. */
        if (is_prime(m))
            return (seq_t)m;
    }
}
