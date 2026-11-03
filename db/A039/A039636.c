/*
 * A039636  Number of steps to fixed point of "n -> n/2 or (n-1)/2 until
 *          result is prime".
 * https://oeis.org/A039636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 1 if is_prime(n); otherwise a(n) = 1 +
 *            a(floor(n/2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9478 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* outside the sequence offset – behaviour irrelevant */
    if (n == 1)
        return 1;          /* the fixed point 1 */
    if (is_prime(n))
        return 1;          /* already a prime – stop here */
    return (seq_t)1 + a(n >> 1);  /* floor division by 2 */
}
