/*
 * A128650  Number of polygons on n vertices with exactly three faces.
 * https://oeis.org/A128650
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-2)/(n-1) * C(2*n-4, n-2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using multiplicative binomial with on‑the‑fly reductions to avoid overflow */

#include "../utils/gcd.h"

/* Compute C(2*k, k) with reductions to keep intermediate values small. */
static seq_t central_binom(idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(k + i);          /* number to multiply in */
        seq_t den = (seq_t)i;                /* denominator to divide by */

        /* Reduce num and den by their gcd first. */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce res with remaining part of den. */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* Multiply and divide in one step (den now divides num*res). */
        res = res * num / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence has offset 2, with a(2) = 0. */
    if (n < 2) return 0;

    idx_t k = n - 2;                         /* k = n-2 */
    seq_t bin = central_binom(k);            /* C(2k, k) = C(2n-4, n-2) */

    seq_t num = (seq_t)4 * (seq_t)k;         /* 4*(n-2) */
    seq_t den = (seq_t)n - 1;                /* (n-1) */

    /* First cancel greatest common divisors with binomial. */
    seq_t g = gcd(bin, den);
    bin /= g;
    den /= g;

    /* Then cancel gcd between num and remaining denominator. */
    g = gcd(num, den);
    num /= g;
    den /= g; /* den should now be 1 */

    return bin * num / den;
}
