/*
 * A174266  Irregular triangle read by rows: T(n,k) = Sum_{i=0..k} (-1)^i *
 *          binomial(3*n+1,i) * binomial(k+3-i,3)^n, 0 <= k <= 3*(n-1).
 * https://oeis.org/A174266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the irregular triangle read by rows,
 *            where T(r,k) = sum_{i=0..k} (-1)^i * binom(3r+1,i) *
 *            binom(k+3-i,3)^r and the flattening follows the order
 *            a(1)=T(1,0), a(2)=T(2,0), …
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 40, u64 n <= 101, u128 n <= 263,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) returning seq_t */

/* fast exponentiation: base^exp in seq_t, exp is idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return 0 (offset is 1) */

    /* find row r such that r*(3r-1)/2 >= n  */
    idx_t r = 1;
    while ((r * (3 * r - 1)) / 2 < n) ++r;

    /* cumulative length before row r */
    idx_t prev = ((r - 1) * (3 * (r - 1) - 1)) / 2;

    /* column index within the row */
    idx_t k = n - prev - 1;

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t i = 0; i <= k; ++i)
    {
        seq_t bin1 = binom(3 * r + 1, i);             /* binom(3r+1,i) */
        seq_t bin3 = binom(k + 3 - i, 3);             /* binom(k+3-i,3) */
        seq_t pw   = pow_seq(bin3, r);                /* bin3^r    */
        seq_t term = bin1 * pw;                       /* individual term */

        if ((i & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    return pos_sum - neg_sum;                         /* final non‑negative result */
}
