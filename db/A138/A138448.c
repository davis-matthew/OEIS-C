/*
 * A138448  a(n) = (prime(n)^6 - prime(n)^2)/15.
 * https://oeis.org/A138448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^6 - prime(n)^2) / 15
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Sequence A138448:
   a(n) = (p(n)^6 - p(n)^2) / 15,
   where p(n) is the n-th prime. */
seq_t a(idx_t n)
{
    /* Retrieve the n-th prime as a 32-bit integer. */
    uint32_t pr = get_prime(n);

    /* Promote to the seq_t type for further arithmetic. */
    seq_t p  = (seq_t)pr;

    /* Compute p^2, p^4, and p^6. */
    seq_t p2 = p * p;
    seq_t p4 = p2 * p2;
    seq_t p6 = p4 * p2;

    /* Compute (p^6 - p^2) / 15. */
    seq_t diff = p6 - p2;
    seq_t result = diff / (seq_t)15;

    return result;
}
