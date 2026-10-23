/*
 * A138411  a(n) = prime(n)^6 - prime(n)^4.
 * https://oeis.org/A138411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^6 - prime(n)^4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 168, u128 n <= 168,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by arithmetic in seq_t, using the provided get_prime function. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* get the n-th prime (n starts at 1) */
    uint32_t p = get_prime(n);

    /* promote to seq_t to preserve precision in the big-ints build */
    seq_t sp = (seq_t)p;

    /* compute p^2, p^4 and p^6 in seq_t */
    seq_t p2 = sp * sp;      /* p^2 */
    seq_t p4 = p2 * p2;      /* p^4 */
    seq_t p6 = p4 * p2;      /* p^6 */

    return p6 - p4;
}
