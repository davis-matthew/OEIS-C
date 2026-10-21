/*
 * A078350  Number of primes in {n, f(n), f(f(n)), ..., 1}, where f is the
 *          Collatz function defined by f(x) = x/2 if x is even; f(x) = 3x + 1
 *          if x is odd.
 * https://oeis.org/A078350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of primes in the Collatz trajectory of n, counting
 *            each prime at most once (2 is counted once)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t v = n;
    while (1) {
        if (isPrime(v))
            count++;
        if (v == 1)
            break;
        if (v & 1) {                   /* odd: 3*v + 1 */
            unsigned __int128 t = (unsigned __int128)v * 3 + 1;
            v = (idx_t)t;
        } else {                        /* even: v / 2 */
            v >>= 1;
        }
    }
    return count;
}
