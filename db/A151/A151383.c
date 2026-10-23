/*
 * A151383  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of 2 n steps
 *          taken from {(-1, -1), (-1, 0), (-1, 1), (1, 1)}.
 * https://oeis.org/A151383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C_n * 3^n where C_n is the Catalan number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, included per build basis */

/* Multiply binomial coefficient binom(2n, n) iteratively */
static seq_t binom_2n_n(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * ((seq_t)n + (seq_t)i) / (seq_t)i;
    }
    return res;
}

/* Catalan number: C_n = binom(2n, n)/(n+1) */
static seq_t catalan(idx_t n)
{
    if (n <= 1) return 1;
    seq_t res = binom_2n_n(n);
    res = res / ((seq_t)n + 1);
    return res;
}

/* Integer power: 3^n */
static seq_t pow3(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        res = res * 3;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t p3  = pow3(n);
    return cat * p3;
}
