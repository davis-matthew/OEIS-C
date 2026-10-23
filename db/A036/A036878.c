/*
 * A036878  a(n) = p^(p-1) where p = prime(n).
 * https://oeis.org/A036878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^(prime(n)-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    seq_t base = (seq_t)p;
    seq_t exp = (seq_t)(p - 1);
    return big_pow(base, exp);
}
