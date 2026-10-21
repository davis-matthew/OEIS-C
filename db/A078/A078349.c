/*
 * A078349  Number of primes in sequence h(m) defined by h(1) = n, h(m+1) =
 *          Floor(h(m)/2).
 * https://oeis.org/A078349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in the sequence h(m) defined by h(1)=n,
 *            h(m+1)=floor(h(m)/2)
 * Range:     u8 n <= 9982, u16 n <= 9982, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"   /* provides bool isPrime(idx_t x) */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; values for n<1 are undefined.
       Handling n<1 simply returns 0 for completeness. */
    if (n < 1) return 0;

    seq_t count = 0;
    idx_t cur = n;
    while (cur > 0) {
        if (isPrime(cur))
            ++count;
        cur >>= 1;              /* floor division by 2 */
    }
    return count;
}
