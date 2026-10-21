/*
 * A086932  Number of non-congruent solutions of x^2 + y^2 == -1 (mod n).
 * https://oeis.org/A086932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏ a(pᵉ) where the product ranges over the * distinct
 *            prime powers pᵉ dividing n, and * * a(2¹) = 2 (a(2ᵉ) = 0 for e >
 *            1) * * a(pᵉ) = (p − 1)·p^(e−1) if p ≡ 1 (mod 4) * * a(pᵉ) = (p +
 *            1)·p^(e−1) if p ≡ 3 (mod 4) * * The function is fully
 *            multiplicative. It implements the * product by factor...
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    unsigned __int128 result = 1;
    uint64_t num = (uint64_t)n;

    /* Handle factor 2: a(2) = 2, higher powers give zero. */
    uint64_t exp2 = 0;
    while ((num & 1ULL) == 0) {
        exp2++;
        num >>= 1;
    }
    if (exp2 > 1)
        return (seq_t)0;
    if (exp2 == 1)
        result *= (unsigned __int128)2;

    /* Factor the remaining odd part. */
    for (uint64_t p = 3; p * p <= num; p += 2) {
        if (num % p == 0) {
            uint64_t exp = 0;
            while (num % p == 0) {
                num /= p;
                exp++;
            }

            /* Compute p^(exp – 1). */
            unsigned __int128 pow = 1;
            for (uint64_t i = 1; i < exp; ++i)
                pow *= (unsigned __int128)p;

            /* Contribute factor according to p mod 4. */
            unsigned __int128 contrib;
            if (p % 4 == 1)
                contrib = (unsigned __int128)(p - 1) * pow;
            else          /* p % 4 == 3 */
                contrib = (unsigned __int128)(p + 1) * pow;

            result *= contrib;
        }
    }

    /* Any remaining prime factor with exponent 1. */
    if (num > 1) {
        uint64_t p = num;
        unsigned __int128 pow = 1;          /* p^(1 – 1) = 1 */
        unsigned __int128 contrib;
        if (p % 4 == 1)
            contrib = (unsigned __int128)(p - 1) * pow;
        else
            contrib = (unsigned __int128)(p + 1) * pow;
        result *= contrib;
    }

    return (seq_t)result;
}
