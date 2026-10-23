/*
 * A135179  p^5 - p^3 - p^2. Exponents are the prime numbers in decreasing
 *          order and p is the n-th prime.
 * https://oeis.org/A135179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p^5 - p^3 - p^2 where p is the n‑th prime number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* An offset of 1 is used in the OEIS entry; return 0 safely for n = 0. */
    if (n == 0) return 0;

    /* Retrieve the n‑th prime using the supplied helper. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Compute p^2, p^3, and p^5 using only integer arithmetic. */
    seq_t p2 = p * p;          /* p^2 */
    seq_t p3 = p2 * p;         /* p^3 */
    seq_t p5 = p3 * p2;        /* p^5 = p^3 * p^2 */

    return p5 - p3 - p2;
}
