/*
 * A138417  a(n) = (prime(n)^4 - prime(n))/2.
 * https://oeis.org/A138417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^4 - prime(n))/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 58, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behaviour below offset is irrelevant */
    seq_t p = (seq_t)get_prime(n);       /* nth prime (1‑based) */
    seq_t p2 = p * p;                    /* p^2 */
    seq_t p4 = p2 * p2;                  /* p^4 */
    return (p4 - p) / 2;                 /* (p^4 - p)/2 */
}
