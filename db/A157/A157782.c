/*
 * A157782  Denominator of Bernoulli(n, -1/2).
 * https://oeis.org/A157782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 55, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of Bernoulli(n, -1/2)
   a(n) = 2^(n-1) * ∏{p prime : p-1 | n} p,
   with 2 omitted when n is odd.
   a(0) = a(1) = 1. */

#include <stdbool.h>

/* Simple primality test for numbers up to a few thousand
   (n + 1 never exceeds the maximum n requested). */
static bool small_is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Multiply by 2^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)2;

    /* Multiply by all primes p such that p-1 divides n
       (exclude p = 2 if n is odd). */
    bool odd = (n & 1);
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor d */
        idx_t p = d + 1;
        if (small_is_prime(p) && !(odd && p == 2))
            result *= (seq_t)p;

        /* Complementary divisor n/d */
        idx_t d2 = n / d;
        if (d2 != d) {
            p = d2 + 1;
            if (small_is_prime(p) && !(odd && p == 2))
                result *= (seq_t)p;
        }
    }

    return result;
}
