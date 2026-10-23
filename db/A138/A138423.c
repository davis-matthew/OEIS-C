/*
 * A138423  a(n) = (prime(n)^4 - prime(n)^3)/2.
 * https://oeis.org/A138423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^4 - prime(n)^3)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 60, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get prime(n) (1‑based) */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^3 and p^4 using seq_t arithmetic */
    seq_t p2 = p * p;          /* p^2 */
    seq_t p3 = p2 * p;         /* p^3 */
    seq_t p4 = p3 * p;         /* p^4 */

    /* (p^4 - p^3) / 2 */
    return (p4 - p3) / 2;
}
