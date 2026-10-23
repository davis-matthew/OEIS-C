/*
 * A101562  a(n) = (-1)^n * coefficient of x^n in Sum_{k>=1}
 *          x^(k-1)/(1+4*x^k).
 * https://oeis.org/A101562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1)^n * sum_{d | n+1} (-4)^(d-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute 4^exp in type seq_t */
static seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    seq_t pos = 0;   /* sum of terms with positive sign */
    seq_t neg = 0;   /* sum of terms with negative sign */

    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            /* First divisor d */
            seq_t val = pow4(d - 1);
            if ( ((n + d - 1) & 1) == 0 )
                pos += val;
            else
                neg += val;

            /* Pair divisor d2 = N/d, if distinct */
            idx_t d2 = N / d;
            if (d2 != d) {
                val = pow4(d2 - 1);
                if ( ((n + d2 - 1) & 1) == 0 )
                    pos += val;
                else
                    neg += val;
            }
        }
    }

    /* The sequence is always non‑negative, so pos >= neg. */
    return pos - neg;
}
