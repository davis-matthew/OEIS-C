/*
 * A153482  a(n) = prime(n)^4 - 8.
 * https://oeis.org/A153482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^4 - 8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);   /* 1‑based index, prime(1)=2 */
    seq_t sq = p * p;                /* p²  – fits in seq_t when a(n) fits */
    return sq * sq - (seq_t)8;       /* p⁴ – 8   */
}
