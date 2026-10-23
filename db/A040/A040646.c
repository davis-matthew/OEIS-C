/*
 * A040646  Continued fraction for sqrt(672).
 * https://oeis.org/A040646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=25; for n≥1 a(n) is the n-th term of the continued fraction
 *            expansion of sqrt(672) (standard algorithm for quadratic surds)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long D  = 672ULL;   /* the radicand */
    const unsigned long long A0 = 25ULL;    /* floor(sqrt(672)) */

    if (n == 0) return (seq_t)A0;

    unsigned long long m     = 0ULL;
    unsigned long long d     = 1ULL;
    unsigned long long term  = A0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * term - m;
        d = (D - m * m) / d;
        term = (A0 + m) / d;
    }
    return (seq_t)term;
}
