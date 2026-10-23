/*
 * A156136  A triangle of polynomial coefficients related to Mittag-Leffler
 *          polynomials: p(x,n)=Sum[Binomial[n, k]*Binomial[n - 1, n -
 *          k]*2^k*x^k, {k, 0, n}]/(2*x).
 * https://oeis.org/A156136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = binom(r+1, c+1) * binom(r, r-c) * 2^c, where r =
 *            floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

/* Compute the OEIS sequence A156136 at index n (n >= 0). */
seq_t a(idx_t n) {
    /* Find the row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t c = n - r * (r + 1) / 2;

    seq_t term1 = binom(r + 1, c + 1);   /* binom(r+1, c+1) */
    seq_t term2 = binom(r, r - c);        /* binom(r, r-c) */
    seq_t two   = pow2(c);                /* 2^c */

    seq_t res = term1;
    res *= term2;
    res *= two;
    return res;
}
