/*
 * A081897  Fourth binomial transform of binomial(n+3, 3).
 * https://oeis.org/A081897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n*(n^3 + 42*n^2 + 407*n + 750)/750
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   pow5_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow5_fast.h"

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;            /* promote n to seq_t */
    seq_t sq   = sn * sn;             /* n^2   */
    seq_t sc   = sq * sn;             /* n^3   */

    /* polynomial part: n^3 + 42*n^2 + 407*n + 750 */
    seq_t poly = sc
               + (seq_t)42 * sq
               + (seq_t)407 * sn
               + (seq_t)750;

    if (n < 3) {
        /* For n=0,1,2 we cancel the 5^n factor from the denominator. */
        seq_t pow5_n = pow5_fast(n);      /* 5^n, fits in 64 bits for n<3 */
        seq_t denom  = (seq_t)750 / pow5_n; /* 750 / 5^n  */
        return poly / denom;              /* exact division */
    } else {
        /* For n≥3 use the simplified form: a(n) = 5^(n-3) * (poly/6) */
        seq_t t = poly / 6;               /* poly is always divisible by 6 */
        seq_t pow5_shift = pow5_fast(n - 3); /* 5^(n-3) */
        return pow5_shift * t;            /* exact multiplication where it fits */
    }
}
