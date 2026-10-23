/*
 * A138442  a(n) = ((n-th prime)^6-(n-th prime^2))/3.
 * https://oeis.org/A138442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-th prime)^6 - (n-th prime)^2)/3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 21, u128 n <= 21,
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
        return 0;          /* values below the offset are undefined */

    uint32_t p = get_prime(n);          /* n-th prime, unsigned 32‑bit */
    seq_t pp = (seq_t)p;                /* promote to seq_t */
    seq_t p2 = pp * pp;                 /* p^2   */
    seq_t p4 = p2 * p2;                 /* p^4   */
    seq_t p6 = p4 * p2;                 /* p^6   */
    return (p6 - p2) / 3;               /* (p^6 - p^2)/3 */
}
