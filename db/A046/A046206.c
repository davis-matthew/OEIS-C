/*
 * A046206  In Leibniz's Harmonic Triangle, write denominator first and then
 *          numerator of each element.
 * https://oeis.org/A046206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = 1 if n is even. If n is odd, let f = (n+1)/2 be the index
 *            of the fraction in the flattened Leibniz Harmonic Triangle. Find
 *            the row r such that r(r+1)/2 >= f (the smallest such r). Then k
 *            = f - r(r-1)/2 - 1 (0‑based position in that row). The
 *            denominator of the fraction is r*binom(r‑1, k),...
 * Range:     u8 n <= 62, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, harmless */
    if ((n & 1) == 0) return 1;    /* numerators are always 1 */

    /* odd n: compute the denominator of the corresponding fraction */
    idx_t f = (n + 1) >> 1;        /* ceil(n/2) */

    idx_t r = 1;
    idx_t triangular = 1;          /* T(1) */
    while (f > triangular) {
        ++r;
        triangular += r;           /* T(r) = r*(r+1)/2 */
    }

    idx_t prev_triangular = triangular - r;   /* T(r-1) */
    idx_t k = f - prev_triangular - 1;         /* 0‑based position */

    seq_t denom = (seq_t)r * binom_uint(r - 1, k);
    return denom;
}
