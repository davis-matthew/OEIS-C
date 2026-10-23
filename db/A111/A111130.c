/*
 * A111130  Numerator of (n+2)^(n+2)/(n+1)^(n+1) - (n+1)^(n+1)/n^n.
 * https://oeis.org/A111130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = ((n+2)^(n+2)*n^n - (n+1)^(2n+2)) / gcd(((n+2)^(n+2)*n^n -
 *            (n+1)^(2n+2)), (n+1)^(n+1)*n^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd_any, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/gcd_any.h"

seq_t a(idx_t n)
{
    /* Special case n = 0, where 0^0 is treated as 1 */
    if (n == 0)
        return (seq_t)3;

    seq_t base_A = (seq_t)(n + 2);
    seq_t base_B = (seq_t)(n + 1);
    seq_t base_C = (seq_t)n;

    seq_t A = pow_seq(base_A, (idx_t)(n + 2));   /* (n+2)^(n+2)   */
    seq_t B = pow_seq(base_B, (idx_t)(n + 1));   /* (n+1)^(n+1)   */
    seq_t C = pow_seq(base_C, (idx_t)n);         /*  n^n          */

    seq_t AC = A * C;            /* (n+2)^(n+2) * n^n   */
    seq_t B2 = B * B;            /* (n+1)^(2n+2)        */
    seq_t N  = AC - B2;          /* numerator before reduction */

    seq_t D = B * C;             /* denominator           */

    seq_t G = gcd_any(N, D);     /* gcd of numerator and denominator */
    return N / G;                /* reduced numerator     */
}
