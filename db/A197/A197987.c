/*
 * A197987  a(n) = prime(n)^(n+1).
 * https://oeis.org/A197987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^(n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t p = (seq_t)get_prime(n);
    seq_t e = (seq_t)(n + 1);
    return big_pow(p, e);
}
