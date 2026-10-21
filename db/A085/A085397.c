/*
 * A085397  Numbers that are not perfect powers and whose squarefree part is
 *          not congruent to 1 (mod 4).
 * https://oeis.org/A085397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085397: numbers that are not perfect powers and whose squarefree part is not congruent to 1 (mod 4). */

#include <stdbool.h>

/* Euclidean algorithm for greatest common divisor of two integers. */
static int my_gcd(int a, int b)
{
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: returns true iff n is a perfect power, and sets *sqfree to the
 * squarefree part of n (product of primes with odd exponent). */
static bool check_perfect_power_and_sqfree(seq_t n, seq_t *sqfree)
{
    if (n <= 1) {
        *sqfree = n;
        return false;
    }

    seq_t temp = n;
    int g = 0;        /* gcd of the prime‑factor exponents */
    seq_t r = 1;      /* squarefree part (product of primes with odd exponent) */

    for (seq_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (g == 0)
                g = exp;
            else
                g = my_gcd(g, exp);
            if (exp & 1)
                r *= p;
        }
    }

    /* Remaining prime factor (if any) has exponent 1. */
    if (temp > 1) {
        if (g == 0)
            g = 1;
        else
            g = my_gcd(g, 1);
        r *= temp;    /* exponent 1 is odd */
    }

    *sqfree = r;
    return g > 1;   /* perfect power if gcd of exponents > 1 */
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    idx_t found = 0;   /* how many valid terms have been found so far */
    seq_t num   = 2;   /* candidate integer */

    while (1) {
        seq_t sqfree;
        if (!check_perfect_power_and_sqfree(num, &sqfree)) {
            if ((sqfree % 4) != 1) {     /* squarefree part not ≡ 1 (mod 4) */
                ++found;
                if (found == n)
                    return num;
            }
        }
        ++num;
    }
}
