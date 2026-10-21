/*
 * A052160  Isolated prime difference equals 6: primes prime(k) such that d(k)
 *          = prime(k+1) - prime(k) = 6 but neither d(k+1) nor d(k-1) is 6.
 * https://oeis.org/A052160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A052160 by scanning primes */
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 0 */

    /* initial window of the first four primes */
    uint64_t p[4] = {2, 3, 5, 7};
    idx_t count = 0;

    for (;;)
    {
        /* differences between consecutive primes in the window */
        uint64_t d1 = p[1] - p[0]; /* p2 - p1 */
        uint64_t d2 = p[2] - p[1]; /* p3 - p2 */
        uint64_t d3 = p[3] - p[2]; /* p4 - p3 */

        /* middle prime p[1] is isolated if its difference is 6 and its
           neighbors differ by 6 are not 6. */
        if (d2 == 6 && d1 != 6 && d3 != 6)
        {
            ++count;
            if (count == n)
                return (seq_t)p[1];
        }

        /* Shift window to the left and insert next prime */
        p[0] = p[1];
        p[1] = p[2];
        p[2] = p[3];
        p[3] = next_prime(p[3]);
    }
}
