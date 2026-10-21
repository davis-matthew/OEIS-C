/*
 * A213669  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the double star graph G(n)
 *          obtained by joining with an edge the centers of two star trees
 *          each having n+1 vertices (n>=1, k>=2).
 * https://oeis.org/A213669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of x^{k+2} in (x^r + x(1+x)^r)^2, where
 *            r = ⌊√n⌋ and k = n − r² (0‑indexed inside the row)
 * Range:     u8 n <= 29, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer floor square root, safe for 64‑bit values. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Binomial coefficient binom(n,k) computed multiplicatively.
   Works for arbitrary‑precision seq_t as long as the type has the
   usual arithmetic operators. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r and column k within that row. */
    idx_t r = isqrt_u64(n);          /* r^2 <= n < (r+1)^2 */
    idx_t k = n - r * r;             /* 0‑based offset inside row (0..2r) */

    /* Exponent e = k + 2 in the domination polynomial. */
    idx_t e = k + 2;

    /* Components of the polynomial. */
    seq_t term = (seq_t)0;

    /* A^2 term: contributes 1 if e == 2r. */
    if (e == 2 * r) {
        term += (seq_t)1;
    }

    /* 2AB term: contributes 2 * binom(r, e - r - 1) if e in [r+1, 2r+1]. */
    if (e >= r + 1 && e <= 2 * r + 1) {
        idx_t i = e - (r + 1);
        term += (seq_t)2 * binom_calc(r, i);
    }

    /* B^2 term: contributes binom(2r, e - 2) if e in [2, 2r+2]. */
    if (e >= 2 && e <= 2 * r + 2) {
        idx_t j = e - 2;
        term += binom_calc(2 * r, j);
    }

    return term;
}
