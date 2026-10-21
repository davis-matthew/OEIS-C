/*
 * A084372  Least composite k such that the nearest integer to the average of
 *          the smallest and largest prime factors of k equals n.
 * https://oeis.org/A084372
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the least composite k such that the nearest integer to
 *            the average of the smallest and largest prime factors of k
 *            equals n. For n >= 2, the following algorithm works: 1) If 2*n-3
 *            is prime, a(n) = 2*(2*n-3). 2) Else if 2*n-2 is prime, a(n) =
 *            2*(2*n-2). 3) Else find the smallest odd prime p...
 * Range:     u8 n <= 26, u16 n <= 495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64-bit integers.
   For our usage 2*n <= 2*10^4 at most (or much larger in arbitrary-precision
   builds), the trial division is sufficient. */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if ((x & 1) == 0) return x == 2;
    for (idx_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* Not defined for n < 2, return 0 */

    idx_t two_n = 2 * n;

    /* Step 1: 2*n-3 prime case (A098090). */
    if (is_prime(two_n - 3))
        return (seq_t)2 * (seq_t)(two_n - 3);

    /* Step 2: 2*n-2 prime case (covers n=2). */
    if (is_prime(two_n - 2))
        return (seq_t)2 * (seq_t)(two_n - 2);

    /* Step 3: find smallest odd prime p ≤ 2*n/2 with
       2*n-p also prime.  The product p*(2*n-p) is then the desired value.
       Since 2*n is even, both p and 2*n-p will be odd primes. */
    for (idx_t p = 3; p <= two_n / 2; p += 2)
    {
        if (is_prime(p) && is_prime(two_n - p))
            return (seq_t)p * (seq_t)(two_n - p);
    }

    /* In theory, a solution always exists (by definition of the series). */
    return 0;
}
