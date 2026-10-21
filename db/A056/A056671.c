/*
 * A056671  1 + the number of unitary and squarefree divisors of n = number of
 *          divisors of reduced squarefree part of n.
 * https://oeis.org/A056671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = 2^{ω_1(n)} where ω_1(n) = number of distinct prime
 *            factors of n with exponent exactly 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_of_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A056671(n).  No external helper functions are required.
   The implementation factors n by trial division and counts only the
   primes whose exponent equals one.  The result is 2 raised to that
   count.  The logic is independent of the actual sizes or types of
   seq_t and idx_t, provided that seq_t supports multiplication by
   the integer literal 2. */

#include <stdint.h>
#include "../utils/power_of_2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by the problem statement */
    uint64_t num = (uint64_t)n;
    uint64_t ones = 0;

    /* handle factor 2 separately */
    uint64_t exp = 0;
    while ((num & 1ULL) == 0) {  /* num % 2 == 0 */
        exp++;
        num >>= 1;
    }
    if (exp == 1)
        ++ones;

    /* odd factors */
    for (uint64_t p = 3; p * p <= num; p += 2) {
        if (num % p == 0) {
            exp = 0;
            while (num % p == 0) {
                num /= p;
                exp++;
            }
            if (exp == 1)
                ++ones;
        }
    }

    /* remaining prime > 1 */
    if (num > 1)
        ++ones;  /* its exponent is 1 */

    return power_of_2(ones);
}
