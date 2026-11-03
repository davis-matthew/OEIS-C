/*
 * A238263  a(n) is the number of ways n can be written in the form
 *          n=2^k1*p1^k2+2^k3*p2^k4, where p1 and p2 are odd prime numbers,
 *          and k1, k2, k3, k4 are nonnegative integers.
 * https://oeis.org/A238263
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1994, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = number of unordered pairs (a,b) with a+b=n, a,b ≥ 1,
 * where each of a and b can be written as 2^k * p^j with p an odd prime,
 * k,j ≥ 0. This is computed by brute‑force scanning of the first ⌊n/2⌋ integers.
 */
#include <stdint.h>
#include <stdbool.h>

/* Determine whether m is a prime power with an odd prime (or 1). */
static bool is_prime_power_odd(uint64_t m)
{
    if (m == 1) return true;                 /* 1 is p^0 for any p */
    uint64_t d = 0;
    for (uint64_t i = 3; i * i <= m; i += 2) /* only odd divisors are needed */
        if ((m % i) == 0) { d = i; break; }

    if (d == 0) return true;                 /* m itself is an odd prime */

    while ((m % d) == 0) m /= d;
    return (m == 1);
}

/* Check if x can be expressed as 2^k * (odd prime)^j with k,j ≥ 0. */
static bool is_representable_uint64(uint64_t x)
{
    if (x == 0) return false;                /* zero cannot appear in the sum */
    while ((x & 1ULL) == 0ULL) x >>= 1;      /* strip all factors of 2 */
    return is_prime_power_odd(x);
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* values below the offset are undefined */

    seq_t count = 0;
    for (idx_t a = 1; a <= n / 2; ++a) {
        idx_t b = n - a;
        if (is_representable_uint64((uint64_t)a) &&
            is_representable_uint64((uint64_t)b))
            ++count;
    }
    return count;
}
