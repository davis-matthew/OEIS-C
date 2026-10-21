/*
 * A094548  Smallest m > n, coprime to n and having as many prime factors as
 *          n, with and without repetitions; a(1)=1.
 * https://oeis.org/A094548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m>n such that GCD(m,n)=1, ω(m)=ω(n) and
 *            Ω(m)=Ω(n) (same number of distinct and total prime factors)
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   bigomega, count_distinct_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 in the OEIS entry */
    if (n <= 1)
        return (seq_t)1;

    /* Number of distinct prime factors of n (ω) and total prime factors (Ω) */
    uint8_t omega_n   = (uint8_t)count_distinct_primes((uint64_t)n);
    uint8_t omega_tot = (uint8_t)bigomega(n);

    for (uint64_t m = n + 1; ; ++m)
    {
        /* must be coprime to n */
        if (gcd((seq_t)m, (seq_t)n) != 1)
            continue;

        /* check both ω and Ω */
        if ((uint8_t)count_distinct_primes((uint64_t)m) == omega_n &&
            (uint8_t)bigomega(m)               == omega_tot)
            return (seq_t)m;
    }
}
