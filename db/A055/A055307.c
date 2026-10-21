/*
 * A055307  Number of labeled rooted trees with n nodes and 6 leaves.
 * https://oeis.org/A055307
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-6)^2 * (n-5)^2 * (n-4) * (n-3) * (n-2) * (n-1) *
 *            (3*n^2 - 29*n + 64) / 8294400 (n ≥ 7)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 7 the sequence is undefined; return 0. */
    if (n < 7) return 0;

    /* Denominator 8294400 = 2^12 * 3^4 * 5^2 (= 8294400). */
    uint64_t den = 8294400ULL;
    seq_t result = 1;

    /* Multiply by n! with cancellation of common factors with denominator */
    uint64_t i;
    for (i = 1; i <= (uint64_t)n; ++i) {
        uint64_t term = i;
        uint64_t g = gcd_uint64(term, den);
        term /= g;
        den /= g;
        result *= (seq_t)term;
    }

    /* Remaining factors of the formula */
    uint64_t n64 = (uint64_t)n;
    uint64_t f1 = n64 - 6;          /* (n-6)^2 */
    uint64_t f2 = n64 - 5;          /* (n-5)^2 */
    uint64_t f3 = n64 - 4;          /* (n-4)  */
    uint64_t f4 = n64 - 3;          /* (n-3)  */
    uint64_t f5 = n64 - 2;          /* (n-2)  */
    uint64_t f6 = n64 - 1;          /* (n-1)  */
    uint64_t f7 = 3*n64*n64 - 29*n64 + 64; /* (3*n^2 - 29*n + 64) */

    /* Multiply squares first (n-6)^2 and (n-5)^2 */
    uint64_t term = f1 * f1;
    uint64_t g = gcd_uint64(term, den);
    term /= g;
    den /= g;
    result *= (seq_t)term;

    term = f2 * f2;
    g = gcd_uint64(term, den);
    term /= g;
    den /= g;
    result *= (seq_t)term;

    /* Multiply the remaining linear factors */
    term = f3; g = gcd_uint64(term, den); term /= g; den /= g; result *= (seq_t)term;
    term = f4; g = gcd_uint64(term, den); term /= g; den /= g; result *= (seq_t)term;
    term = f5; g = gcd_uint64(term, den); term /= g; den /= g; result *= (seq_t)term;
    term = f6; g = gcd_uint64(term, den); term /= g; den /= g; result *= (seq_t)term;
    term = f7; g = gcd_uint64(term, den); term /= g; den /= g; result *= (seq_t)term;

    /* After all cancellations, the denominator must be 1. */
    /* For safety, ignore any remaining denominator on the assumption
       that the formula guarantees integrality. */

    return result;
}
