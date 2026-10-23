/*
 * A138437  a(n) = ((n-th prime)^5-(n-th prime)^3)/12.
 * https://oeis.org/A138437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5-(p_n)^3)/12 (p_n is the n‑th prime, p_1=2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence offset is 1 */

    uint32_t p = get_prime(n);        /* retrieve the n‑th prime */
    seq_t sp = (seq_t)p;              /* cast to seq_t */

    seq_t p2 = sp * sp;               /* p^2 */
    seq_t p3 = p2 * sp;               /* p^3 */
    seq_t p5 = p3 * p2;               /* p^5 = p^3 * p^2 */

    return (p5 - p3) / 12;
}
