/*
 * A087032  a(n) = 1 if 2*A151800(n) - n is prime, otherwise 0, where
 *          A151800(n) is the smallest prime > n.
 * https://oeis.org/A087032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if 2*A151800(n) - n is prime, otherwise 0
 * Range:     u8 n <= 2060, u16 n <= 9505, u32 n <= 9505, u64 n <= 9505,
 *            u128 n <= 9505, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* smallest prime strictly greater than n */
    uint64_t p = next_prime((uint64_t)n);
    /* compute 2*p - n */
    seq_t x = (seq_t)2 * (seq_t)p - (seq_t)n;
    /* return 1 if x is prime, else 0 */
    return is_prime_big(x) ? (seq_t)1 : (seq_t)0;
}
