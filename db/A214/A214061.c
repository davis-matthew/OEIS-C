/*
 * A214061  Least m>0 such that gcd(2*n-1+m, 2*n-m) > 1.
 * https://oeis.org/A214061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{p | (4n-1)} (p+1)/2, where p is an odd prime divisor
 *            of 4n-1. Implementation finds the smallest prime factor of
 *            (4n-1) and returns (p+1)/2. This follows from gcd(2n-1+m, 2n-m)
 *            > 1 iff some odd prime p divides 4n-1 and m ≡ (p+1)/2 (mod p).
 *            The smallest such m is (p+1)/2
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type, though unused here */

/* Helper: find the smallest odd prime divisor of an odd number x.
   If x is prime, returns x itself. */
static seq_t smallest_prime_factor(seq_t x)
{
    /* x is odd and >=3 for n >= 1. */
    seq_t i;

    for (i = 3; i <= x / i; i += 2) {
        if (x % i == 0)
            return i;
    }
    /* x is prime. */
    return x;
}

seq_t a(idx_t n)
{
    /* Compute x = 4n - 1.  Since n >= 1, x is odd and >= 3. */
    seq_t x = (seq_t)n * 4 - 1;

    /* Find smallest prime divisor p of x. */
    seq_t p = smallest_prime_factor(x);

    /* a(n) = (p + 1) / 2.  The division is exact since p is odd. */
    return (p + 1) / 2;
}
