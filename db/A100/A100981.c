/*
 * A100981  Number of totally ramified extensions over Q_5 with degree n in
 *          the algebraic closure of Q_5.
 * https://oeis.org/A100981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(Sum_{s=0..m} 5^s*(5^{eps(s)*n}-5^{eps(s-1)*n}))
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 49, u64 n <= 99, u128 n <= 224,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where m = v5(n) (the exponent of 5 in n). */

#include <stdint.h>
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    /* compute m = exponent of 5 in n */
    idx_t temp = n;
    idx_t m = 0;
    while (temp % 5U == 0U)
    {
        temp /= 5U;
        ++m;
    }

    seq_t total = 1;          /* s = 0 term */
    seq_t pow5s_seq = 1;      /* 5^s as seq_t */
    idx_t  pow5s_int = 1;     /* 5^s as idx_t for exponent calculation */
    seq_t power_prev = 1;     /* 5^{e_{s-1}} as seq_t */

    for (idx_t s = 1; s <= m; ++s)
    {
        pow5s_seq *= 5;     /* 5^s */
        pow5s_int *= 5;     /* 5^s as integer */

        /* e_s = n * (pow5s_int - 1) / (4 * pow5s_int) */
        idx_t numerator = n * (pow5s_int - 1U);
        idx_t denom     = 4U * pow5s_int;
        idx_t e_s       = numerator / denom;

        seq_t power_curr = pow_seq_t((seq_t)5, e_s);
        seq_t diff       = power_curr - power_prev;

        total += pow5s_seq * diff;

        power_prev = power_curr;
    }

    return (seq_t)n * total;
}
