/*
 * A098096  Numbers of the form p^2 * 2^p for p prime.
 * https://oeis.org/A098096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n^2 * 2^p_n * where p_n is the n-th prime (p_1 = 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 */
        return 0;

    /* obtain the n-th prime */
    uint32_t prime = get_prime(n);

    /* compute p^2 * 2^p */
    seq_t p2      = (seq_t)prime * (seq_t)prime;   /* p^2   */
    seq_t two_pow = pow2((idx_t)prime);           /* 2^p   */

    return p2 * two_pow;
}
