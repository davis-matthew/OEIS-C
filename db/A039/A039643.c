/*
 * A039643  Number of steps to fixed point of "k -> k/2 or (k-1)/2 until
 *          result is prime", starting with prime(n)+1.
 * https://oeis.org/A039643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: start with k = prime(n)+1; repeatedly divide k by 2 (floor)
   until k is prime; count the number of terms visited, including the first
   term. */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence; guard for safety. */
    if (n == 0) return 0;

    seq_t k   = (seq_t)get_prime(n) + 1;
    seq_t cnt = 1;                /* count the starting value */

    while (!is_prime_big(k))
    {
        k = k / 2;                /* floor division by 2 */
        ++cnt;
    }

    return cnt;
}
