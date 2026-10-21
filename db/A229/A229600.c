/*
 * A229600  Number of defective 3-colorings of an n X 2 0..2 array connected
 *          horizontally and vertically with exactly two mistakes, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=6; for n>2: a(n)=3^(n-5) * (31*n+32*n^2-30)
 *            (equivalently a(n) = (31*n+32*n^2-30) / 3^(5-n) for n=3,4)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* compute 3^exp for non‑negative exp */
static seq_t pow3(size_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    seq_t n_se  = (seq_t)n;
    seq_t factor = (seq_t)32 * n_se * n_se + (seq_t)31 * n_se - (seq_t)30;

    if (n >= 5) {
        seq_t p = pow3((size_t)(n - 5));
        return factor * p;
    } else { /* n is 3 or 4 */
        size_t denom_exp = (size_t)(5 - n);
        seq_t p = pow3(denom_exp);
        return factor / p;
    }
}
