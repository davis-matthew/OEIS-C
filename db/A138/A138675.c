/*
 * A138675  a(n) = prime(n)^6 mod prime(n-1).
 * https://oeis.org/A138675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^6 mod prime(n-1)
 * Range:     u8 n <= 54, u16 n <= 6618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  For n < 2 the
       behavior is unspecified, so we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Retrieve the n-th prime and the (n-1)-th prime. */
    uint32_t p   = get_prime(n);
    uint32_t pm1 = get_prime(n - 1);

    /* Compute p^6 mod pm1.  The exponent is small, so we can use
       the provided modular exponentiation routine. */
    return mod_pow((seq_t)p, 6, (seq_t)pm1);
}
