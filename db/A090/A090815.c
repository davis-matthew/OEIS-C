/*
 * A090815  a(n)=denominator(B(2*prime(n))) where prime(n)=n-th prime and B(k)
 *          denotes the k-th Bernoulli number.
 * https://oeis.org/A090815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 if 2*prime(n)+1 is not prime, // otherwise a(n) =
 *            6*(2*prime(n)+1)
 * Range:     u8 n <= 8, u16 n <= 725, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    // index n is 1-based ; get_prime expects 1-based index
    seq_t p = (seq_t)get_prime(n);          // p = prime(n)
    seq_t q = p * (seq_t)2 + (seq_t)1;      // q = 2p + 1

    if (is_prime((idx_t)q))
        return q * (seq_t)6;                // 6 * q
    else
        return (seq_t)6;                    // 6
}
