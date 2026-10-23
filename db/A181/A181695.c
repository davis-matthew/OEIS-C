/*
 * A181695  Triangle read by rows: T(n,m) = number of solutions x_1 + x_2 +
 *          ... + x_k <= n, where 1 <= x_i <= m, and any k >= 1.
 * https://oeis.org/A181695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = Sum_{i=0..⌊n/(m+1)⌋}
 *            binomial(n-mi,i)*(-1)^i*2^(n-(m+1)i) - 1 * a(n) is the triangle
 *            read by rows of T(n,m) with offset 1
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* locate the row `row` and column `col` from the linear index `n` */
    idx_t row = 1;
    while ((seq_t)row * (row + 1) / 2 < n) ++row;
    idx_t col = n - ((idx_t)(row - 1) * row / 2);   /* 1‑based column */

    /* compute T(row,col) by the alternating‑sign binomial formula */
    idx_t max_i = row / (col + 1);
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t arg   = row - col * i;                       /* n - m*i */
        idx_t expo  = row - (col + 1) * i;                 /* n - (m+1)i */
        seq_t bin   = binom(arg, i);                       /* binomial(arg,i)   */
        seq_t p2    = pow2(expo);                         /* 2^expo            */
        seq_t term  = bin * p2;                           /* product (no overflow in arbitrary‑precision build) */

        if (i & 1)
            sum_odd += term;
        else
            sum_even += term;
    }

    seq_t result = sum_even - sum_odd;   /* guaranteed non‑negative */
    return result - 1;
}
