/*
 * A132350  If n > 1 is a k-th power with k >= 2 then a(n) = 0, otherwise a(n)
 *          = 1.
 * https://oeis.org/A132350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1 or n>1 that is not a perfect power, otherwise
 *            a(n) = 0. For n>1: factor n, compute the gcd of the exponents in
 *            its prime factorisation; if that gcd ≥ 2 then n is a perfect
 *            power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Smallest prime factor of a 64‑bit integer.
   Simple trial division is fine for the limits of this sequence */
static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return i;
    return n; /* n is prime */
}

/* Greatest common divisor of two non‑negative ints */
static int gcd_int(int a, int b)
{
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1ULL)                 /* a(1) = 1, a(0) is irrelevant */
        return (seq_t)1;

    uint64_t x = (uint64_t)n;
    int gcd_exp = 0;               /* gcd of exponents so far */

    while (x > 1ULL) {
        uint64_t p = _smallest_prime_factor(x);
        int exp = 0;
        while (x % p == 0ULL) {
            x /= p;
            ++exp;
        }

        if (gcd_exp == 0)
            gcd_exp = exp;
        else
            gcd_exp = gcd_int(gcd_exp, exp);

        if (gcd_exp == 1)          /* early exit: cannot be a perfect power */
            break;
    }

    return (seq_t)(gcd_exp >= 2 ? 0 : 1);
}
