/*
 * A091591  Number of pairs of twin primes between n^2 and (n+1)^2.
 * https://oeis.org/A091591
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091591 – number of twin‑prime pairs between n^2 and (n+1)^2
 *
 *  a(n) = |{ (p,p+2) : n^2 ≤ p, p+2 ≤ (n+1)^2,  p and p+2 prime }| 
 *
 *  This routine uses the supplied is_prime(idx_t) routine to test
 *  primality.  For n ≥ 3 it scans the interval [n², (n+1)² − 2] and
 *  increments a seq_t counter for each twin pair found.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3; below that the value is
     * unspecified, but we return 0 to keep the function total. */
    if (n < 3)
        return (seq_t)0;

    idx_t low  = n * n;                /* first value to test          */
    idx_t high = (n + 1) * (n + 1);    /* upper bound (inclusive)      */

    seq_t count = 0;
    /* p runs from low to high-2 so that p+2 ≤ high */
    for (idx_t p = low; p + 2 <= high; ++p)
    {
        if (is_prime(p) && is_prime(p + 2))
            ++count;
    }
    return count;
}
