/*
 * A070975  Number of steps to reach 1 in `3x+1' (or Collatz) problem starting
 *          with prime(n).
 * https://oeis.org/A070975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = number of Collatz steps needed to reach 1 when starting
 *            from the n-th prime (prime(n))
 * Range:     u8 n <= 10, u16 n <= 1130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined, but protect */
    seq_t x = get_prime(n);               /* prime(n) */
    seq_t steps = 0;
    while (x != 1) {
        if ((x & 1) != 0) {
            x = x * 3 + 1;                 /* odd step: 3x + 1 */
        } else {
            x >>= 1;                       /* even step: x / 2 */
        }
        ++steps;
    }
    return steps;
}
