/*
 * A117961  Hexagonal numbers with prime indices.
 * https://oeis.org/A117961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = A000040(n) * (2*A000040(n)-1)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* offset is 1, undefined for n <= 0 */
    uint32_t p32 = get_prime(n);           /* nth prime (1‑based) */
    seq_t p   = (seq_t)p32;                /* cast to result type */
    seq_t two = (seq_t)2;
    return p * (two * p - (seq_t)1);
}
