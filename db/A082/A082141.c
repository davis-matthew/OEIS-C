/*
 * A082141  A transform of C(n,7).
 * https://oeis.org/A082141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(binomial(n+7,7) * 2^(n-1))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 39, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd of two 64‑bit integers (used only for factor cancelling) */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Special‑case n = 0 (2^(n‑1) is ½, ceil gives 1) */
    if (n == 0)
        return (seq_t)1;

    /* Build numerator and denominator lists for C(n+7,7) */
    uint64_t num[7];
    uint64_t den[7];
    for (idx_t i = 0; i < 7; ++i) {
        num[i] = n + 7 - i;   /* n+7, n+6, … , n+1 */
        den[i] = i + 1;       /* 1,2,…,7          */
    }

    /* Cancel common factors to keep intermediate products small. */
    for (idx_t i = 0; i < 7; ++i) {
        for (idx_t j = 0; j < 7; ++j) {
            uint64_t g = gcd_uint64(num[i], den[j]);
            if (g > 1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    /* Multiply the remaining numerators using the seq_t type. */
    seq_t binom = (seq_t)1;
    for (idx_t i = 0; i < 7; ++i)
        binom *= (seq_t)num[i];

    /* Compute the product of the remaining denominators (fits in 64‑bit). */
    uint64_t denProd = 1;
    for (idx_t i = 0; i < 7; ++i)
        denProd *= den[i];

    /* Exact division – the binomial coefficient is an integer. */
    binom /= (seq_t)denProd;

    /* Compute 2^(n‑1) by repeated multiplication by 2. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow2 *= (seq_t)2;

    /* Final product gives a(n). */
    return binom * pow2;
}
