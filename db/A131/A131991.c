/*
 * A131991  a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3.
 * https://oeis.org/A131991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint32_t p = get_prime(n);   /* nth prime, 1-based */
    seq_t sip   = (seq_t)p;      /* p */
    seq_t sp2   = sip * sip;     /* p^2 */
    seq_t sp3   = sp2 * sip;     /* p^3 */
    return (seq_t)1 + sip + sp2 + sp3;
}
