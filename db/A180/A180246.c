/*
 * A180246  Triangle T(n,k) read by rows: T(n,k) = Sum_{j=0..k} (-1)^j
 *          *binomial(n+1,j)*(k+2-j)^n, 0 <= k < n.
 * https://oeis.org/A180246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(n,k) = ∑_{j=0}^{k} (-1)^j * binomial(n+1,j) * (k+2-j)^n, with
 *            0 ≤ k < n. The sequence a(n) is obtained by reading the triangle
 *            row‑by‑row (1‑based)
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 83, i64 n <= 218,
 *            i128 n <= 608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                 /* for completeness */

/* Fast power for seq_t: base^exp with exp as idx_t. */
static seq_t pow_seq_custom(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Locate the row that contains the nth term (rows are 1‑based). */
    idx_t row = 1, cum = 1;                      /* cumulative terms */
    while (cum < n)
    {
        ++row;
        cum = row * (row + 1) / 2;                /* total terms up to this row */
    }

    /* Column index within the row (0‑based). */
    idx_t prev = (row - 1) * row / 2;
    idx_t k    = n - prev - 1;

    /* Compute the defining sum for this (row,k). */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j)
    {
        /* binomial(row+1, j) computed multiplicatively. */
        seq_t bin = 1;
        for (idx_t i = 1; i <= j; ++i)
        {
            bin = bin * (seq_t)(row + 2 - i) / (seq_t)i;
        }

        seq_t base = (seq_t)(k + 2 - j);
        seq_t pw   = pow_seq_custom(base, row);

        if (j & 1)
            sum = sum - bin * pw;
        else
            sum = sum + bin * pw;
    }
    return sum;
}
