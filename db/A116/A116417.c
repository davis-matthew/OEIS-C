/*
 * A116417  If n = Sum_{m>=1} 2^(m-1) * b(n,m), where each b(n,m) is 0 or 1
 *          and the sum is a finite sum, then a(n) = denominator of Sum_{m>=1}
 *          b(n,m)/m.
 * https://oeis.org/A116417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of ∑_{m≥1, bit m of n is 1} 1/m
 * Range:     u8 n <= 54, u16 n <= 3677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the sum */
    seq_t den = 1;   /* current denominator of the sum */
    idx_t m = 1;     /* 1‑based bit position */
    idx_t bits = n;  /* work on a copy of n */

    while (bits)
    {
        if (bits & 1ULL)
        {
            seq_t m_seq = (seq_t)m;
            seq_t num2 = num * m_seq + den;
            seq_t den2 = den * m_seq;
            seq_t g = gcd(num2, den2);
            num = num2 / g;
            den = den2 / g;
        }
        bits >>= 1;
        ++m;
    }
    return den;   /* for n = 0, returns 1 as required */
}
