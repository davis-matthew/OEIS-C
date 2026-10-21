/*
 * A062812  a(n) = Sum_{i=1..n} i^(n - i) + (-1)^(n - i)*(n - i)^i.
 * https://oeis.org/A062812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} ( i^(n-i) + (-1)^(n-i) * (n-i)^i )
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine computes a single term of sequence A062812.
   It uses binary exponentiation on seq_t values; seq_t supports
   the usual arithmetic operators.  For the arbitrary‑precision
   build seq_t is a signed big integer type, so the signed
   subtraction in the sum is legal.  For fixed‑size builds
   the test data only involve n for which the intermediate
   values fit in the chosen type.  All other limits are
   respected by the conditions of the building system. */

#include <stdint.h>

/* Helper: binary exponentiation for seq_t (signed or unsigned integer types). */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t factor = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= factor;
        exp >>= 1;
        if (exp)
            factor *= factor;
    }
    return result;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* n must be >= 1; behaviour for n=0 is undefined. */
    seq_t total = (seq_t)0;
    idx_t i;
    for (i = 1; i <= n; ++i)
    {
        idx_t p1 = n - i;          /* exponent for i */
        idx_t p2 = i;              /* exponent for (n-i) */
        seq_t term1 = pow_seq_t((seq_t)i, p1);
        seq_t term2 = pow_seq_t((seq_t)(n - i), p2);
        /* Add i^(n-i) */
        total += term1;
        /* Add or subtract (n-i)^i depending on parity of (n-i) */
        if (p1 & 1)
            total -= term2;
        else
            total += term2;
    }
    return total;
}
