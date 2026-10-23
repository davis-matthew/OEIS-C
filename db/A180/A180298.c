/*
 * A180298  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-8.
 * https://oeis.org/A180298
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of solutions to x1+…+xn = n with 0≤xi≤n-8 and max xi =
 *            n-8. This is computed as a(n) = F(n,n-8) – F(n,n-9), where
 *            F(n,m) = Σ_{k=0}^{⌊n/(m+1)⌋} (-1)^k · C(n,k) · C(n – k(m+1)+n-1
 *            , n-1)
 * Range:     u8 none, u16 none, u32 n <= 36, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A180298 */

#include <stdint.h>
#include "../utils/binom.h"

static seq_t count_leq(idx_t boxes, idx_t N, idx_t max_val)
/* Count #solutions to x_1+…+x_boxes = N, 0≤x_i≤max_val. */
{
    idx_t bucket = max_val + 1U;          /* m+1                */
    idx_t max_k = N / bucket;             /* floor division     */
    seq_t sum_even = 0, sum_odd = 0;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = binom(boxes, k);
        idx_t rem = N - k * bucket;
        /* rem + boxes - 1 choose boxes - 1  */
        term *= binom(rem + boxes - 1, boxes - 1);
        if (k & 1U)
            sum_odd += term;
        else
            sum_even += term;
    }
    return sum_even - sum_odd;
}

seq_t a(idx_t n)
{
    if (n < 9U)
        return 0;

    /* F(n, n-8) and F(n, n-9) */
    seq_t f_max   = count_leq(n, n, n - 8U);
    seq_t f_lower = count_leq(n, n, n - 9U);

    return f_max - f_lower;
}
