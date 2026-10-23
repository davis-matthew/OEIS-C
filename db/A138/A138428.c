/*
 * A138428  a(n) = (prime(n)^5 - prime(n))/10.
 * https://oeis.org/A138428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^5 - prime(n)) / 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = get_prime(n);          /* prime(n) */
    seq_t pow5 = p * p * p * p * p;  /* p^5 */
    return (pow5 - p) / 10;
}
