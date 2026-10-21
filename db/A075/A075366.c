/*
 * A075366  Smallest product (n+1)(n+2)...(n+k) that is divisible by the
 *          product of all the primes up to n.
 * https://oeis.org/A075366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = (2p)! / n! where p is the largest prime ≤ n, except
 *            a(10)=360360. This is equivalent to a(n) = ∏_{i=n+1}^{2p} i
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Prime tester: deterministic for n < 10^6 (our n ≤ 10000)   */
/* ------------------------------------------------------------ */
static bool is_prime(uint32_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    if (x % 5 == 0) return x == 5;
    if (x % 7 == 0) return x == 7;
    if (x % 11 == 0) return x == 11;
    if (x % 13 == 0) return x == 13;
    if (x % 17 == 0) return x == 17;
    if (x % 19 == 0) return x == 19;
    if (x % 23 == 0) return x == 23;
    if (x % 29 == 0) return x == 29;
    if (x % 31 == 0) return x == 31;
    if (x % 37 == 0) return x == 37;
    if (x % 41 == 0) return x == 41;
    if (x % 43 == 0) return x == 43;
    if (x % 47 == 0) return x == 47;
    if (x % 53 == 0) return x == 53;
    if (x % 59 == 0) return x == 59;
    if (x % 61 == 0) return x == 61;
    if (x % 67 == 0) return x == 67;
    if (x % 71 == 0) return x == 71;
    if (x % 73 == 0) return x == 73;
    if (x % 79 == 0) return x == 79;
    if (x % 83 == 0) return x == 83;
    if (x % 89 == 0) return x == 89;
    if (x % 97 == 0) return x == 97;

    /* trial division by odd numbers up to sqrt(x) */
    for (uint32_t d = 101; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Return the largest prime ≤ n (n ≥ 2).                         */
/* ------------------------------------------------------------ */
static uint32_t largest_prime_leq(uint32_t n)
{
    for (uint32_t v = n; v >= 2; --v) {
        if (is_prime(v)) return v;
    }
    return 2; /* fallback, unreachable for n≥2 */
}

/* ------------------------------------------------------------ */
/* The sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1  */
    if (n <= 1) return (seq_t)1;

    /* special case noted in OEIS */
    if (n == 10) return (seq_t)360360;

    /* largest prime p ≤ n */
    uint32_t p = largest_prime_leq((uint32_t)n);
    uint32_t upper = 2 * p;

    seq_t prod = (seq_t)1;
    for (uint32_t i = (uint32_t)n + 1; i <= upper; ++i)
        prod *= (seq_t)i;

    return prod;
}
