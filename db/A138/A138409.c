/*
 * A138409  a(n) = prime(n)^6 - prime(n)^2.
 * https://oeis.org/A138409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^6 - prime(n)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function retrieves the n-th prime and computes p^6 - p^2 using
   only integer arithmetic. For 64‑bit builds the value will be correct
   as long as it fits in seq_t. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p32 = get_prime(n);          /* nth prime, 1‑based */
    seq_t p   = (seq_t)p32;              /* cast to seq_t */
    seq_t p2  = p * p;                   /* p^2   */
    seq_t p4  = p2 * p2;                 /* p^4   */
    seq_t res = p4 * p2 - p2;            /* p^6 - p^2 */
    return res;
}
