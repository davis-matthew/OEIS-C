/*
 * A138443  a(n) = ((n-th prime)^6-(n-th prime^2))/4.
 * https://oeis.org/A138443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6 - (p_n)^2)/4, where p_n is the n-th prime
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);   /* n-th prime (1‑based) */
    seq_t p2 = p * p;                /* p^2  */
    seq_t p6 = p2 * p2 * p2;         /* p^6  */
    return (p6 - p2) / 4;            /* (p^6 - p^2)/4  */
}
