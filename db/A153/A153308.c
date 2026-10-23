/*
 * A153308  Numbers k such that 10*k+7 is not prime.
 * https://oeis.org/A153308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 10*k + 7 is composite (i.e.
 *            not prime)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simply scans k = 1,2,3,..., testing 10*k+7 for primality,
   increments a counter when the value is composite, and returns the k that
   yields the nth such composite.  The routine uses the provided isPrimeGeneric
   test for arbitrary-precision integers. */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;

    while (1)
    {
        seq_t val = (seq_t)10 * k + 7;        /* 10*k + 7 */
        if (!isPrimeGeneric(val))            /* composite? */
        {
            ++count;
            if (count == n)
                return (seq_t)k;            /* found the nth term */
        }
        ++k;
    }
}
