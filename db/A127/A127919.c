/*
 * A127919  1/3 of product of three numbers: the n-th prime, the previous
 *          number and the following number.
 * https://oeis.org/A127919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^3 - prime(n))/3
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 346, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* 0 is a safe fallback for out‑of‑range indices */
    seq_t p = (seq_t)get_prime(n);  /* nth prime */
    seq_t p2 = p * p;               /* p^2 */
    seq_t p3 = p2 * p;              /* p^3 */
    seq_t diff = p3 - p;            /* p^3 - p */
    seq_t result = diff / 3;        /* divide by 3 */
    return result;
}
