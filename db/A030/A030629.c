/*
 * A030629  Numbers with 11 divisors.
 * https://oeis.org/A030629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^10
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 914,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, prevents underflow */
    seq_t p = (seq_t)get_prime(n); /* nth prime */
    return big_pow(p, (seq_t)10);  /* compute prime^10 */
}
