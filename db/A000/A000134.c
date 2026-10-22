/*
 * A000134  Positive zeros of Bessel function of order 0 rounded to nearest
 *          integer.
 * https://oeis.org/A000134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(j_{0,n}), the n-th positive zero of J_0, computed
 *            with McMahon's asymptotic expansion: with b = (n - 1/4)*Pi,
 *            j_{0,n} ~ b + 1/(8b) - 124/(3(8b)^3) + 120928/(15(8b)^5) -
 *            401743168/(105(8b)^7) + 1071187749376/(315(8b)^9)
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const double PI = 3.14159265358979323846264338327950288;
    double b = ((double)n - 0.25) * PI;
    double w = 1.0 / (8.0 * b);
    double w2 = w * w;
    double corr;
    double x;
    unsigned long long r;

    if (n == 1) {
        /* j_{0,1} = 2.404825557695773... (expansion is least accurate here) */
        return (seq_t)2;
    }
    corr = w * (1.0
               + w2 * (-124.0 / 3.0
               + w2 * (120928.0 / 15.0
               + w2 * (-401743168.0 / 105.0
               + w2 * (1071187749376.0 / 315.0)))));
    x = b + corr;
    r = (unsigned long long)(x + 0.5);
    return (seq_t)r;
}
