/*
 * A069549  Smallest composite k such that phi(k) > k*(1-1/n).
 * https://oeis.org/A069549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (nextprime(n))^2 where nextprime(n) is the smallest prime
 *            ≥ n
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* should not happen for OEIS offset 1 */

    uint64_t p = (uint64_t)n;           /* search for next prime ≥ n                */
    while (!is_prime(p)) ++p;

    seq_t sq = (seq_t)p * (seq_t)p;     /* square the prime, result in seq_t       */
    return sq;
}
