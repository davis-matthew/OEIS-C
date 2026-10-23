/*
 * A138455  a(n) = ((n-th prime)^6-(n-th prime)^4)/4.
 * https://oeis.org/A138455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6-(p_n)^4)/4, where p_n is the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 309,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                 /* for uint32_t */
#include "../utils/get_prime.h"     /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term given above */
    if (n == 1)
        return (seq_t)12;           /* (2^6 - 2^4)/4 = (64-16)/4 = 12 */

    /* For n >= 2, p is odd and (p^2-1) is divisible by 4 */
    uint32_t p = get_prime(n);      /* n-th prime, n >= 1 */
    seq_t sp  = (seq_t)p;           /* promote to seq_t */

    seq_t p2 = sp * sp;             /* p^2 */
    seq_t t  = (p2 - (seq_t)1) / (seq_t)4;  /* (p^2-1)/4  */

    seq_t p4 = p2 * p2;             /* p^4 */
    return p4 * t;                  /* p^4 * (p^2-1)/4 = (p^6 - p^4)/4 */
}
