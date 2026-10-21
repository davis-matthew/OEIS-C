/*
 * A081899  Third binomial transform of binomial(n+4, 4).
 * https://oeis.org/A081899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(n^4 + 58*n^3 + 971*n^2 + 5114*n + 6144)/6144
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is indexed from 0 */
    seq_t n0 = (seq_t)n;
    seq_t n_sq = n0 * n0;                     /* n^2 */
    seq_t n_cu = n_sq * n0;                   /* n^3 */
    seq_t n_q  = n_cu * n0;                   /* n^4 */

    seq_t poly = n_q
               + (seq_t)58 * n_cu
               + (seq_t)971 * n_sq
               + (seq_t)5114 * n0
               + (seq_t)6144;                 /* polynomial part */

    if (n >= 5) {
        /* 4^n / 1024 = 4^(n-5)  (since 4^5 = 1024) */
        seq_t pow = 1;
        for (idx_t i = 0; i < n - 5; ++i) pow *= (seq_t)4;

        /* divide polynomial by 6 (exact) before the final multiplication */
        seq_t poly_div6 = poly / 6;
        return pow * poly_div6;
    } else {
        /* 4^n is small for n < 5, safe to multiply directly */
        seq_t pow = 1;
        for (idx_t i = 0; i < n; ++i) pow *= (seq_t)4;
        return (pow * poly) / 6144;
    }
}
