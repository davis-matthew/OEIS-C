/*
 * A089002  Number of non-congruent solutions to x^2 + 2y^2 == -1 (mod n).
 * https://oeis.org/A089002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089002: number of non‑congruent solutions to x² + 2y² ≡ –1 (mod n).
    The sequence is multiplicative.
    For a prime power p⁽ᵉ⁾ (p odd):
        a(p⁽ᵉ⁾) = (p – 1)·p⁽ᵉ⁻¹⁾   if  p ≡ 1 or 3 (mod 8)
        a(p⁽ᵉ⁾) = (p + 1)·p⁽ᵉ⁻¹⁾   if  p ≡ 5 or 7 (mod 8)
    For powers of two:
        a(2⁰) = 1, a(2¹) = 2, a(2²) = 4,
        a(2ᵉ) = 0   for e > 2
    The function below factors n by trial division (fast for n ≤ 10⁴)
    and then applies the formula.  No external helpers are required. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 1 by definition   */
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;       /* remaining part to factor   */
    seq_t    result = (seq_t)1;        /* multiplicative accumulator */

    /* --- factor 2 separately ------------------------------------ */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0ULL) {    /* temp divisible by 2   */
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        if (e <= 2) {                 /* a(2ᵉ) = 2ᵉ          */
            seq_t part = ((seq_t)1) << e;   /* 2^e   */
            result *= part;
        } else {                      /* a(2ᵉ) = 0 (e>2)   */
            return (seq_t)0;           /* whole product 0    */
        }
    }

    /* --- trial division by odd numbers -------------------------- */
    uint64_t d = 3;
    while (d <= temp / d) {
        if (temp % d == 0ULL) {
            uint64_t p = d;
            uint64_t exp = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                ++exp;
            }

            /* compute part for odd prime power p^exp */
            seq_t coeff = ((p % 8ULL == 1ULL) || (p % 8ULL == 3ULL))
                          ? (seq_t)(p - 1ULL) : (seq_t)(p + 1ULL);

            /* p^(exp-1) */
            seq_t power = (seq_t)1;
            for (uint64_t j = 1; j < exp; ++j)
                power *= (seq_t)p;

            seq_t part = coeff * power;
            result *= part;
        }
        d += 2;                      /* next odd divisor   */
    }

    /* --- remaining prime > sqrt(n) -------------------------------- */
    if (temp > 1ULL) {               /* a remaining prime factor with exponent 1   */
        seq_t coeff = ((temp % 8ULL == 1ULL) || (temp % 8ULL == 3ULL))
                      ? (seq_t)(temp - 1ULL) : (seq_t)(temp + 1ULL);
        result *= coeff;             /* p^(1-1)=1, multiply by coeff            */
    }

    return result;
}
