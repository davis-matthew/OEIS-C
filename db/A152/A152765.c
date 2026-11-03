/*
 * A152765  Smallest prime divisor of Catalan number A000108(n), with a(0) =
 *          a(1) = 1.
 * https://oeis.org/A152765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of Catalan number C(n) * with a(0)
 *            = a(1) = 1. For n ≥ 2, a(n) = 2 unless n+1 is a power * of two
 *            (i.e. n = 2^k – 1). Then a(n) is the least odd prime that *
 *            divides C(n). The computation uses Legendre’s formula to test *
 *            divisibility without forming the huge Catalan...
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9712 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

/* Test whether n+1 is a power of two (n = 2^k – 1).  n is unsigned. */
static bool is_power_of_two_minus_one(idx_t n)
{
    idx_t x = n + 1;
    return x != 0 && (x & (x - 1)) == 0;
}

/* Legendre exponent of prime p in m!  (m is idx_t, p is uint64_t). */
static unsigned long long exp_in_factorial(idx_t m, uint64_t p)
{
    unsigned long long sum = 0;
    while (m)
    {
        m /= p;
        sum += m;
    }
    return sum;
}

/* Smallest prime divisor of Catalan(n). */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Even Catalan numbers have smallest divisor 2. */
    if (!is_power_of_two_minus_one(n))
        return (seq_t)2;

    /* n = 2^k – 1: Catalan number is odd.  Find first odd prime p that
     * divides C(n) using Legendre’s formula. */
    for (uint64_t p = 3; ; p += 2)
    {
        /* Skip non‑primes – cheap trial test. */
        if (!is_prime_seq((seq_t)p))
            continue;

        /* Exponent of p in C(n) = C(2n,n)/(n+1) . Since n+1 is a power
         * of two, p ∤ (n+1), so exp is exp(2n,p) – 2·exp(n,p). */
        unsigned long long e = exp_in_factorial(2 * n, p)
                           - 2 * exp_in_factorial(n, p);

        if (e > 0)
            return (seq_t)p;
    }
}
