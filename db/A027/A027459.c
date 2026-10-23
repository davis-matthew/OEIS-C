/*
 * A027459  Numerator of Sum_{k=1..n} H(k)/k, where H(k) is k-th harmonic
 *          number.
 * https://oeis.org/A027459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator( Sum_{k=1..n} H(k)/k ) where H(k) =
 *            Sum_{i=1..k} 1/i. The implementation keeps the least common
 *            multiple of 1..n (denoted L) and the numerators of the harmonic
 *            numbers and the harmonic square sums. After the main loop it
 *            reduces the final fraction (P^2 + Q) / (2*L^2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Variables:
       L   - lcm(1..k) after processing k
       L2  - L*L  (denominator for harmonic square sums)
       P   - numerator of H(k) with denominator L
       Q   - numerator of H2(k) = Sum 1/i^2 with denominator L^2
    */
    seq_t L = 1;      /* lcm so far */
    seq_t L2 = 1;     /* L*L */
    seq_t P = 0;      /* numerator of H(k) */
    seq_t Q = 0;      /* numerator of H2(k) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;            /* cast k to seq_t */
        seq_t g = gcd(L, kseq);           /* gcd(L, k) */
        seq_t mult = kseq / g;            /* k / g  (small int) */
        seq_t mult_sq = mult * mult;      /* (k/g)^2  (small int) */

        /* Update numerator of H(k)   : P = P*(k/g) + L/g  */
        seq_t L_div_g = L / g;            /* L/g  (big) */
        P = P * mult + L_div_g;

        /* Update numerator of H2(k)
           Q = Q*(k/g)^2 + L^2/(g^2)   */
        seq_t L2_div_g2 = L2 / (g * g);   /* L^2/(g^2)   (big) */
        Q = Q * mult_sq + L2_div_g2;

        /* Advance L and L2 to next values */
        L = L * mult;                     /* L_new = L_old*(k/g) */
        L2 = L2 * mult_sq;                /* L2_new = L2_old*(k/g)^2 */
    }

    /* Final fraction: (P^2 + Q) / (2 * L^2) */
    seq_t Psq = P * P;
    seq_t R = Psq + Q;                     /* numerator before reduction */
    seq_t D = L2 * 2;                      /* denominator before reduction */

    seq_t g_final = gcd(R, D);
    return R / g_final;
}
