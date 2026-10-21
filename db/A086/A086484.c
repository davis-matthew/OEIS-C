/*
 * A086484  Let k be the largest number such that n is a k-th power; then a(n)
 *          is the least positive number m such that m + n is a (k+1)st power.
 * https://oeis.org/A086484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next (k+1)‑th power after n minus n where k = gcd of
 *            exponents in the prime factorisation of n (k = 0 for n = 1). For
 *            n = 1, a(1) = 1 because the next 1st power after 1 is 2
 * Range:     u8 n <= 242, u16 n <= 9372, u32 n <= 9372, u64 n <= 9372,
 *            u128 n <= 9372, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* ---------- helpers ---------- */

/* Greatest common divisor (Euclid). */

/* Test whether base^exp <= limit (no 128‑bit overflow). */
static bool pow_leq_uint64(uint64_t base, uint64_t exp, uint64_t limit)
{
    __uint128_t res = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        if (res > (__uint128_t)limit / base) return false;
        res *= base;
    }
    return true;
}

/* Floor of n^(1/e) for 64‑bit n and integer e>=1.
   Binary search over [1, n]. */
static uint64_t floor_root_uint64(uint64_t n, uint64_t e)
{
    if (e == 1) return n;
    uint64_t low = 1, high = n;
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if (pow_leq_uint64(mid, e, n))
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute base^exp using exponentiation by squaring.  Result fits in uint64_t. */
static uint64_t pow_uint64(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    uint64_t power = base;
    while (exp) {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* n is guaranteed >=1 by the specification.  Treat n == 0 as a no‑op. */
    if (n == 0) return (seq_t)0;

    /* Special case: n == 1 -> k = 0, next 1st power is 2. */
    if (n == 1) return (seq_t)1;

    /* Factorisation of n to obtain exponents. */
    uint64_t temp = (uint64_t)n;
    uint64_t k = 0;
    bool first = true;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (first) { k = exp; first = false; }
            else k = gcd_uint64(k, exp);
        }
    }
    if (temp > 1) {          /* remaining prime factor with exponent 1 */
        if (first) { k = 1; first = false; }
        else k = gcd_uint64(k, 1);
    }

    /* Compute the next (k+1)-th power. */
    uint64_t e = k + 1;                     /* exponent for the next power */
    uint64_t root = floor_root_uint64((uint64_t)n, e);
    uint64_t base = root + 1;                /* smallest integer whose e‑th power is > n */
    uint64_t pow_val = pow_uint64(base, e);  /* base^e fits in uint64_t for our n range */

    return (seq_t)(pow_val - (uint64_t)n);
}
