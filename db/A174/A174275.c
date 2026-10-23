/*
 * A174275  a(n) = 2^(n-1) mod M(n) where M(n) = A014963(n) is the exponential
 *          of the Mangoldt function.
 * https://oeis.org/A174275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a power of an odd prime (p > 2), otherwise 0
 * Range:     u8 n <= 8597, u16 n <= 8597, u32 n <= 8597, u64 n <= 8597,
 *            u128 n <= 8597, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 0, a(5) = 1, … */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* 1 is not a prime power */

    if (n == 2)
        return 0;          /* the only even prime is not allowed */

    if ((n & 1) == 0)
        return 0;          /* even numbers cannot be powers of odd primes */

    /* Find the smallest prime divisor of n (if any). */
    idx_t p = 0;
    idx_t m = n;
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0)                /* n itself is prime */
        p = m;

    /* Verify that n consists solely of the prime factor p. */
    idx_t tmp = n;
    while (tmp % p == 0)
        tmp /= p;

    return (seq_t)(tmp == 1 ? 1 : 0);
}
