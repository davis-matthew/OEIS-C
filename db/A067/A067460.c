/*
 * A067460  a(n) = mu(prime(n)-1)+1.
 * https://oeis.org/A067460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = mu(prime(n)-1)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    idx_t prime_n = get_prime(n);
    idx_t value    = prime_n - 1;                 /* prime(n)-1  */
    seq_t mu       = mobius(value);               /* Möbius function of prime(n)-1 */
    return mu + 1;                                /* a(n) = mu(...) + 1 */
}
