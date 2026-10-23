/*
 * A101808  Number of primes between two consecutive even numbers.
 * https://oeis.org/A101808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = count of primes p with 2n <= p <= 2n+2. For n = 0, a(0) =
 *            1 (prime 2). For n = 1, a(1) = 2 (primes 2 and 3). For n >= 2,
 *            a(n) = 1 if 2n+1 is prime, otherwise 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* [0,2] contains the prime 2 */
    if (n == 1) return (seq_t)2;          /* [2,4] contains primes 2 and 3 */

    unsigned __int128 val = (unsigned __int128)n * 2 + 1;
    uint64_t p = (uint64_t)val;

    return is_prime(p) ? (seq_t)1 : (seq_t)0;
}
