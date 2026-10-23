/*
 * A127922  1/24 of product of three numbers: n-th prime, previous and
 *          following number.
 * https://oeis.org/A127922
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n * (p_n^2 - 1) / 24 where p_n is the n-th prime (p_1 =
 *            2, p_2 = 3, ...). The sequence starts at n = 2 with a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 633, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 2) the behavior is undefined. */
    if (n < 2)
        return 0;

    /* Obtain the n-th prime (odd for n >= 3). */
    uint32_t pk = get_prime(n);

    /* Promote to a wider type to avoid overflow during the
       intermediate multiplication before the division by 24. */
    unsigned __int128 p = (unsigned __int128)pk;
    unsigned __int128 num = p * (p * p - 1);
    num /= 24;

    return (seq_t)num;
}
