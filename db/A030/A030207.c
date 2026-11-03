/*
 * A030207  Expansion of eta(q)^2 * eta(q^2) * eta(q^4) * eta(q^8)^2 in powers
 *          of q.
 * https://oeis.org/A030207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 72, i16 n <= 9898, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9302 (2026-11-01)
 * Helpers:   ipow_u64_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030207: Multiplicative function defined on prime powers
 * a(2^e) = (-2)^e
 * a(p^e) = 0 if p≡5,7 (mod 8) and e odd,
 *          = p^e      if p≡5,7 (mod 8) and e even
 * a(p^e) for p≡1,3 (mod 8):
 *    let p = x^2 + 2y^2 (unique with x,y≥0),
 *    a(p) = 4x^2 – 2p
 *    a(p^e) = a(p)*a(p^(e-1)) – p^2*a(p^(e-2)) for e≥2
 * The function is multiplicative over prime powers.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_u64_u32.h"

/* Find integers x,y s.t. p = x^2 + 2*y^2, using brute force.
   The prime satisfies p ≡ 1,3 (mod 8), so a solution exists. */
static seq_t a_prime(unsigned long p)
{
    for (uint64_t x = 0; x * x <= p; ++x)
    {
        uint64_t rem = p - x * x;
        if ((rem & 1ULL) != 0) continue;          /* must be even */
        uint64_t y2 = rem / 2ULL;
        for (uint64_t y = 0; y * y <= y2; ++y)
        {
            if (y * y == y2)
            {
                seq_t xx = (seq_t)x;
                seq_t pp = (seq_t)p;
                return 4 * xx * xx - 2 * pp;
            }
        }
    }
    /* Should never reach here for valid p */
    return 0;
}

static seq_t a_prime_power(uint64_t p, uint32_t e)
{
    if (p == 2ULL)
    {
        seq_t res = 1;
        for (uint32_t i = 0; i < e; ++i) res *= -2;
        return res;
    }

    uint64_t mod8 = p & 7ULL;
    if (mod8 == 5ULL || mod8 == 7ULL)
    {
        /* even exponent -> p^e, odd exponent -> 0 */
        return (e & 1U) ? 0 : ipow_u64_u32(p, e);
    }

    /* p ≡ 1 or 3 (mod 8) : recurrence with base a(p) */
    seq_t ap = a_prime((unsigned long)p);      /* a(p) */
    if (e == 1U) return ap;

    seq_t ap2 = ap * ap - (seq_t)p * (seq_t)p;   /* a(p^2) */
    if (e == 2U) return ap2;

    seq_t prev2 = ap;      /* a(p^1) */
    seq_t prev1 = ap2;     /* a(p^2) */
    seq_t base = ap;
    const seq_t p2 = (seq_t)p * (seq_t)p;

    for (uint32_t k = 3U; k <= e; ++k)
    {
        seq_t cur = base * prev1 - p2 * prev2;   /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* factor 2 */
    uint32_t e2 = 0;
    while ((m & 1ULL) == 0ULL && m > 0ULL)
    {
        ++e2;
        m >>= 1;
    }
    if (e2)
        result *= a_prime_power(2ULL, e2);

    /* odd prime factors */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL)
    {
        if (m % p == 0ULL)
        {
            uint32_t e = 0;
            while (m % p == 0ULL)
            {
                ++e;
                m /= p;
            }
            result *= a_prime_power(p, e);
        }
    }

    /* remaining prime > sqrt(m) */
    if (m > 1ULL)
        result *= a_prime_power(m, 1U);

    return result;
}
