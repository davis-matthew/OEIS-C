/*
 * A082924  For n prime, a(n) = n+1; for n even and > 2, a(n) = n/2; otherwise
 *          a(n) = 3*n.
 * https://oeis.org/A082924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082924 : For n≥1, a(n) = n+1 if n is prime, a(n)=n/2 if n>2 and even,
   otherwise a(n)=3n. */

#include <stdint.h>
#include "../utils/_is_prime.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (_is_prime((uint64_t)n))
        return sn + (seq_t)1;
    if (n > (idx_t)2 && (n & (idx_t)1) == (idx_t)0)
        return sn >> 1;           /* n/2 */
    return sn * (seq_t)3;
}
