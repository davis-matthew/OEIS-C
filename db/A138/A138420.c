/*
 * A138420  a(n) = ((prime(n))^4-(prime(n))^2)/4.
 * https://oeis.org/A138420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n))^4-(prime(n))^2)/4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the nth prime (1‑indexed) as a seq_t. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute p^2 and p^4 using only integer operations. */
    seq_t p2 = p * p;     /* p^2 */
    seq_t p4 = p2 * p2;   /* p^4 */

    /* Return (p^4 - p^2) / 4. */
    return (p4 - p2) / 4;
}
