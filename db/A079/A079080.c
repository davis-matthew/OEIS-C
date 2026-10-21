/*
 * A079080  a(n) = gcd((prime(n)+1)*(prime(n+1)+1)/4, prime(n)*prime(n+1)+1).
 * https://oeis.org/A079080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd((prime(n)+1)*(prime(n+1)+1)/4 ,
 *            prime(n)*prime(n+1)+1)
 * Range:     u8 n <= 6, u16 n <= 56, u32 n <= 7128, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, local_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 1 (a(1) is the first listed term) */

#include "../utils/get_prime.h"
#include "../utils/local_gcd.h"

seq_t a(idx_t n)
{
    /* Retrieve the n‑th and (n+1)-th primes.  get_prime(1) = 2. */
    seq_t p  = get_prime(n);
    seq_t q  = get_prime(n + 1);

    /* Compute the two arguments of the gcd.  The product in term1 is
       guaranteed to be divisible by 4 because p+1 and q+1 are even
       for consecutive primes (except when p=2, then p+1=3 odd, but
       q+1 is even and the product is still divisible by 2; integer
       division truncates correctly). */
    seq_t term1 = ((p + 1) * (q + 1)) / 4;
    seq_t term2 = p * q + 1;

    return local_gcd(term1, term2);
}
