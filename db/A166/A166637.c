/*
 * A166637  Totally multiplicative sequence with a(p) = 7*(p-1) for prime p.
 * https://oeis.org/A166637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e∥n} (7·(p−1))^e
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9642 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1, n==0 is unused */
    if (n == 1)
        return (seq_t)1;            /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;     /* work on a 64‑bit copy */
    seq_t result = (seq_t)1;

    while (temp > 1)
    {
        uint64_t p = smallest_prime_factor(temp);   /* prime divisor */
        uint64_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            exp++;
        }

        seq_t base = (seq_t)7 * ((seq_t)p - (seq_t)1);
        seq_t pow = (seq_t)1;
        for (uint64_t i = 0; i < exp; ++i)
            pow *= base;

        result *= pow;
    }

    return result;
}
