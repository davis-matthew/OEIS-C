/*
 * A079395  a(n) = prime(n)^11.
 * https://oeis.org/A079395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^11
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 16, u128 n <= 450,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    uint32_t p = get_prime(n);                /* nth prime as uint32_t */
    return big_pow((seq_t)p, (seq_t)11);      /* compute p^11 in seq_t */
}
