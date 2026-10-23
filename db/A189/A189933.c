/*
 * A189933  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=(sin(Pi/5))^2,
 *          t=(cos(Pi/5))^2.
 * https://oeis.org/A189933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sec²(π/5)⌋ + ⌊n*tan²(π/5)⌋ Simplified to: a(n) =
 *            12*n - 2 - 2*⌊2*n*√5⌋ Because tan²(π/5) = 5 – 2√5 and sec²(π/5)
 *            = 1 + tan²(π/5)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7144, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1, safety guard */

    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* t = 12*n - 2  */
    seq_t t = seq_mul((seq_t)12, N);      /* 12*n   */
    t = t - (seq_t)2;                    /* 12*n-2 */

    /* m = 2*n  */
    seq_t m = seq_mul((seq_t)2, N);

    /* prod = 5 * m * m   (gives 5*(2n)^2)  */
    seq_t prod = seq_mul(m, m);          /* m*m   */
    prod = seq_mul(prod, (seq_t)5);      /* 5*(m*m) */

    /* sqrt_floor = ⌊√prod⌋ = ⌊2n*√5⌋ */
    seq_t sqrt_floor = floor_sqrt(prod);

    /* two_sqrt = 2 * sqrt_floor */
    seq_t two_sqrt = seq_mul(sqrt_floor, (seq_t)2);

    /* result = t - two_sqrt = 12*n - 2 - 2*⌊2n*√5⌋ */
    seq_t res = t - two_sqrt;
    return res;
}
