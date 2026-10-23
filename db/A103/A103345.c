/*
 * A103345  Numerator of Sum_{k=1..n} 1/k^6 = zeta(6,n).
 * https://oeis.org/A103345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of ∑_{k=1..n} 1/k⁶ (sequence A103345)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- local Euclidean GCD for unsigned seq_t (works for all widths) --- */
static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Numerator of the nth partial sum of 1/k⁶   */
seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator               */
    seq_t den = 1;   /* current denominator              */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute k⁶  without overflow on the given seq_t type */
        seq_t k6 = (seq_t)k;
        for (int i = 1; i < 6; ++i)
            k6 *= (seq_t)k;

        /* reduce the new fraction   */
        seq_t g   = my_gcd(den, k6);          /* gcd(den, k6)            */
        seq_t lcm_factor = k6 / g;            /* LCM factor for denominators */

        seq_t new_num = num * lcm_factor + den / g;
        seq_t new_den = den * lcm_factor;

        seq_t g2 = my_gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;
}
