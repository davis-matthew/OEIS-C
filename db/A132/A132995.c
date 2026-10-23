/*
 * A132995  a(n) = gcd(Sum_{k=1..n} prime(k), Product{j=1..n} prime(j)).
 * https://oeis.org/A132995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd( sum_{k=1..n} prime(k), product_{j=1..n} prime(j) )
 * Range:     u8 n <= 13, u16 n <= 158, u32 n <= 2006, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe guard */

    /* Compute the sum of the first n primes. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += (seq_t)get_prime(k);
    }

    /* Compute the product of the first n primes modulo that sum. */
    seq_t prod_mod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod_mod = (prod_mod * (seq_t)get_prime(k)) % sum;
    }

    /* The required value is the gcd of the sum and the product. */
    return gcd(sum, prod_mod);
}
