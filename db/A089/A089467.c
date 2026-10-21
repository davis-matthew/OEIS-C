/*
 * A089467  Hyperbinomial transform of A089466 and also the inverse
 *          hyperbinomial transform of A089468.
 * https://oeis.org/A089467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..⌊n/2⌋} (-1)^r * n!/(r! * 2^r * (n-2r)!) *
 *            (n+1)^{n-2r}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* base = n+1 */
    seq_t base = (seq_t)(n + 1);
    /* power = (n+1)^n  */
    seq_t power = big_pow(base, (seq_t)n);
    /* factor starts with the term for r = 0 : n!/(0! * 1 * n!) = 1 */
    seq_t factor = 1;
    seq_t ans_pos = 0, ans_neg = 0;
    int sign = 1;                     /* 1 for positive, -1 for negative */
    idx_t maxR = n / 2;               /* floor(n/2) */
    seq_t base_sq = base * base;      /* (n+1)^2, small integer */

    for (idx_t r = 0; r <= maxR; ++r) {
        /* term = factor * power */
        seq_t term = factor * power;
        if (sign > 0)
            ans_pos += term;
        else
            ans_neg += term;

        if (r == maxR)
            break;                  /* last term processed */

        /* Update factor for next r:
           factor_{r+1} = factor_r * (n-2r)*(n-2r-1) / ((r+1)*2) */
        idx_t A = n - 2 * r;
        idx_t B = A - 1;
        seq_t mult = (seq_t)(A * B);             /* small product, fits 64‑bit */
        seq_t num   = factor * mult;
        seq_t denom = (seq_t)((r + 1) * 2);
        factor = num / denom;                    /* exact division */

        /* Update power: divide by (n+1)^2 to reduce exponent by 2 */
        power = power / base_sq;

        sign = -sign;  /* alternate sign */
    }

    return ans_pos - ans_neg;
}
