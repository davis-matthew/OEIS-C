/*
 * A118335  a(n)= smallest multiple of (prime(n+1)-prime(n)) which is >=
 *          prime(n+1), where prime(m) is the m-th prime.
 * https://oeis.org/A118335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of (prime(n+1)-prime(n)) that is >=
 *            prime(n+1)
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prime_n   = (seq_t)get_prime(n);     /* nth prime (n >= 1) */
    seq_t prime_np1 = (seq_t)get_prime(n + 1); /* (n+1)th prime */
    seq_t diff      = prime_np1 - prime_n;     /* difference of consecutive primes */
    seq_t k         = (prime_np1 + diff - 1) / diff; /* ceil(prime_np1 / diff) */
    return k * diff;       /* smallest multiple of diff >= prime_np1 */
}
