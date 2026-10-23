/*
 * A137486  Numbers with 23 divisors.
 * https://oeis.org/A137486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^(22)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 16,
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
        return (seq_t)0;
    seq_t p = get_prime(n);
    /* 22 fits easily in seq_t, use it as exponent */
    return big_pow(p, (seq_t)22);
}
