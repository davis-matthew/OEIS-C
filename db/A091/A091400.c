/*
 * A091400  a(n) = Product_{ odd primes p | n } (1 + Legendre(-1,p) ).
 * https://oeis.org/A091400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{odd primes p | n} (1 + Legendre(-1,p))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Legendre(-1,p) = 1 if p ≡ 1 mod 4,  -1 if p ≡ 3 mod 4, 0 if p = 2.
   Therefore a(n) = 2^k where k = number of distinct odd primes ≡ 1 mod 4 dividing n,
   and a(n) = 0 if n has any odd prime ≡ 3 mod 4.  a(n) = 1 if n has no odd primes. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remove all factors of 2 – they do not affect the product. */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    /* If after removing 2's the number is 1, no odd prime factors. */
    if (m == 1)
        return (seq_t)1;

    /* Any factor 3 immediately forces the product to 0. */
    if (m % 3 == 0)
        return (seq_t)0;

    int count = 0;               /* number of odd primes ≡ 1 (mod 4) */
    /* Trial division using the 6k±1 pattern.  
       Inside the loop m keeps decreasing, so we recompute the bound.   */
    for (idx_t p = 5; p <= m / p; p += 6) {
        /* Check the prime candidate p = 6k - 1 */
        if (m % p == 0) {
            if (p % 4 == 3)
                return (seq_t)0;
            count++;
            while (m % p == 0)
                m /= p;
        }
        /* Check the prime candidate q = 6k + 1 */
        idx_t q = p + 2;
        if (q > m)
            continue;
        if (m % q == 0) {
            if (q % 4 == 3)
                return (seq_t)0;
            count++;
            while (m % q == 0)
                m /= q;
        }
    }

    /* If a prime factor remains > 1, it is a single odd prime. */
    if (m > 1) {
        /* m is odd by construction, check its residue modulo 4. */
        if (m % 4 == 3)
            return (seq_t)0;
        count++;
    }

    /* Result equals 2^count.  Use left shift to avoid overflow in seq_t. */
    seq_t res = (seq_t)1;
    while (count--)
        res <<= 1;
    return res;
}
